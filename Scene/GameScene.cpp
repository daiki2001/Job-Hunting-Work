#include "GameScene.h"
#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/EngineGeneral.h"
#include "./Input/GameInput.h"
#include "./Header/Camera.h"
#include "./Header/Parameter.h"
#include "UI.h"

const std::wstring GameScene::gameResourcesDir = L"./Resources/Game/";
const std::string GameScene::stageDir = resourcesDirectory + "Stage/";
Player* GameScene::player = Player::Get();
Stage* GameScene::stage = Stage::Get();
bool GameScene::isClear = false;

GameScene::GameScene(SceneChanger* sceneChanger) :
	BaseScene(sceneChanger),
	background(FUNCTION_ERROR),
	backgroundObj(FUNCTION_ERROR),
	clear(FUNCTION_ERROR)
{
	Init();
	Reset();
}

GameScene::~GameScene()
{
#ifdef _DEBUG
	std::string msg = typeid(*this).name() + std::string("から抜けました。\n");
	OutputDebugStringA(msg.c_str());
#endif // _DEBUG
}

void GameScene::Init()
{
	auto draw = Library::DrawPolygon::GetInstance();

	if (background == FUNCTION_ERROR)
	{
		background = draw->LoadTextrue((gameResourcesDir + L"Floor.png").c_str());
	}
	if (backgroundObj == FUNCTION_ERROR)
	{
		const float TEX_SIZE = 64.0f;

		backgroundObj = draw->CreateRect(static_cast<float>(DirectXInit::GetInstance()->windowWidth),
										 static_cast<float>(DirectXInit::GetInstance()->windowHeight));
		draw->Tiring(backgroundObj,
					 DirectXInit::GetInstance()->windowWidth / TEX_SIZE,
					 DirectXInit::GetInstance()->windowHeight / TEX_SIZE);
	}
	if (clear == FUNCTION_ERROR)
	{
		clear = draw->LoadTextrue((gameResourcesDir + L"Clear.png").c_str());
	}

	player->Init();

	Camera::targetRadius = 10.0f;
	Camera::longitude = Math::DEGREE_F * (-90.0f);
	Camera::latitude = Math::DEGREE_F * (0.0f);

	Camera::pos = { 0.0f, 0.0f, -10.0f };
	Camera::target = Area::INIT_CAMERA;
	Camera::upVec = { 0.0f, 1.0f, 0.0f };
}

void GameScene::Update()
{
	if (isClear)
	{
		if (isSceneDest == false && GameInput::Get()->DecisionTrigger())
		{
			isSceneDest = true;
			nextScene = SceneChanger::Scene::Title;
			changeAnimation.Start();
		}
	}
	else
	{
		player->Update(GameInput::Get());
		stage->Update();

		if (Stage::IsGoal())
		{
			isClear = true;
		}
	}

#ifdef _DEBUG
	if (Input::IsKeyTrigger(DIK_R))
	{
		Reset();
	}
	if (Input::IsKeyTrigger(DIK_L))
	{
		stage->LastRoom();
	}
#endif // _DEBUG
	if (Input::IsKeyTrigger(DIK_RETURN))
	{
		isSceneDest = true;
		nextScene = SceneChanger::Scene::Title;
		changeAnimation.Start();
	}

	if (isSceneDest)
	{
		if (changeAnimation.GetChange())
		{
			sceneChanger->SceneChange(nextScene, true);
		}
	}
}

void GameScene::Draw()
{
	Library::DrawPolygon::GetInstance()->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);

	// 3Dオブジェクト
	if (stage->scroll.GetFlag())
	{
		stage->ScrollDraw();
	}
	else
	{
		stage->Draw();
	}
	player->Draw();
}

void GameScene::BGDraw()
{
	DirectXInit* w = DirectXInit::GetInstance();
	int winW = w->windowWidth;
	int winH = w->windowHeight;
	auto draw = Library::DrawPolygon::GetInstance();

	draw->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);
	DirectDrawing::ChangeSpriteShader();

	draw->DrawTextrue(winW / 2.0f,
					  winH / 2.0f,
					  static_cast<float>(winW),
					  static_cast<float>(winH),
					  0.0f,
					  Parameter::Get("white1x1"),
					  DirectX::XMFLOAT2(0.5f, 0.5f),
					  Color::AddAlphaValue(Color::BLACK, 1.0f));
}

void GameScene::UIDraw()
{
	DirectXInit* w = DirectXInit::GetInstance();
	int winW = w->windowWidth;
	int winH = w->windowHeight;
	auto draw = Library::DrawPolygon::GetInstance();

	draw->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);
	DirectDrawing::ChangeSpriteShader();

	if (isClear)
	{
		draw->DrawTextrue(winW / 2.0f, winH / 2.0f, 160.0f * 4.0f, 48.0f * 4.0f, 0.0f, clear,
						  DirectX::XMFLOAT2(0.5f, 0.5f), Color::AddAlphaValue(Color::BLUE, 1.0f));
	}
	else
	{
		const float INVENTORY_SCALE = 1.5f;
		const float TEX_SIZE = 32.0f;
		player->DrawInventory(winW - static_cast<int>(TEX_SIZE * INVENTORY_SCALE * 2.0f),
							  winH - static_cast<int>(TEX_SIZE * INVENTORY_SCALE * 4.0f), INVENTORY_SCALE);

		UI::Get()->WASD(winW - TEX_SIZE * 9.0f, winH - TEX_SIZE * 3.0f, 0.5f * INVENTORY_SCALE);
		UI::Get()->UpDownArrow(winW - TEX_SIZE * INVENTORY_SCALE * 3.0f, winH - TEX_SIZE * INVENTORY_SCALE * 3.0f, 0.5f * INVENTORY_SCALE);
		UI::Get()->Space(winW - TEX_SIZE * INVENTORY_SCALE * 2.0f, winH - TEX_SIZE * INVENTORY_SCALE * 1.0f, 0.5f * INVENTORY_SCALE);
	}
	draw->DrawString(0.0f, winH - (32.0f * (0.0f + 1.0f)), 2.0f,
					 Color::AddAlphaValue(Color::WHITE, 1.0f), Color::AddAlphaValue(Color::BLACK, 0.5f),
					 "Title:F1");
}

void GameScene::Reset()
{
	isClear = false;
	player->Reset();
	stage->Reset();
}

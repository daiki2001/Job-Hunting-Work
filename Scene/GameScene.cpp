#include "GameScene.h"
#include "./Header/DirectXInit.h"
#include "./Input/GameInput.h"
#include "./Header/Camera.h"

const std::wstring GameScene::gameResourcesDir = L"./Resources/Game/";
const std::string GameScene::stageDir = resourcesDirectory + "Stage/";
Player* GameScene::player = Player::Get();
Stage* GameScene::stage = Stage::Get();
bool GameScene::isClear = false;

GameScene::GameScene(SceneChanger* sceneChanger) :
	BaseScene(sceneChanger),
	background(Engine::FUNCTION_ERROR),
	clear(Engine::FUNCTION_ERROR)
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
	if (background == FUNCTION_ERROR)
	{
		background = draw->LoadTextrue((gameResourcesDir + L"background.png").c_str());
	}
	if (clear == FUNCTION_ERROR)
	{
		clear = draw->LoadTextrue((gameResourcesDir + L"Clear.png").c_str());
	}

	player->Init(draw);

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
	DirectXInit* w = DirectXInit::GetInstance();
	int winW = w->windowWidth;
	int winH = w->windowHeight;

	draw->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);

	// 背景

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

	// 前景
	DirectDrawing::ChangeSpriteShader();
	if (isClear)
	{
		draw->DrawTextrue(winW / 2.0f, winH / 2.0f, 160.0f * 4.0f, 48.0f * 4.0f, 0.0f, clear,
						  DirectX::XMFLOAT2(0.5f, 0.5f), Color::AddAlphaValue(Color::BLUE, 1.0f));
	}
	else
	{
		const float inventoryScale = 1.5f;
		player->DrawInventory(static_cast<int>(winW - 64.0f * inventoryScale), static_cast<int>(winH - 32.0f * inventoryScale), inventoryScale);

		draw->DrawString(0.0f, winH - (32.0f * (3.0f + 1.0f)), 2.0f, Color::AddAlphaValue(Color::WHITE, 1.0f),
						 "Move:WASD");
		draw->DrawString(0.0f, winH - (32.0f * (2.0f + 1.0f)), 2.0f, Color::AddAlphaValue(Color::WHITE, 1.0f),
						 "ItemSelect:Arrow");
	}
	draw->DrawString(0.0f, winH - (32.0f * (1.0f + 1.0f)), 2.0f, Color::AddAlphaValue(Color::WHITE, 1.0f),
					 "Action:Space");
	draw->DrawString(0.0f, winH - (32.0f * (0.0f + 1.0f)), 2.0f, Color::AddAlphaValue(Color::WHITE, 1.0f),
					 "Title:F1");
}

void GameScene::Reset()
{
	isClear = false;
	player->Reset();
	stage->Reset();
}

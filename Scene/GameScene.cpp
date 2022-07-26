#include "GameScene.h"
#include "./Header/DirectXInit.h"
#include "./ShaderMgr/ShaderManager.h"
#include "./Header/Input.h"
#include "./Header/Camera.h"
#include "Player.h"
#include "./Stage/Stage.h"

namespace
{
static Player* player = Player::Get();
static Stage* stage = Stage::Get();
}

const std::wstring GameScene::gameResourcesDir = L"./Resources/Game/";

GameScene::GameScene(SceneChenger* sceneChenger) :
	BaseScene(sceneChenger),
	background(Engine::FUNCTION_ERROR)
{
	Init();
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	background = draw.LoadTextrue((gameResourcesDir + L"background.png").c_str());
	player->Init(&draw);
	stage->Init(&draw);
	stage->LoadStage("./Resources/Game/Stage/test.csv");

	Camera::targetRadius = 10.0f;
	Camera::longitude = Math::DEGREE_F * (-90.0f);
	Camera::latitude = Math::DEGREE_F * (0.0f);

	Camera::pos = { 0.0f, 0.0f, -10.0f };
	Camera::target = { 7.0f, -3.0f, 0.0f };
	Camera::upVec = { 0.0f, 1.0f, 0.0f };
}

void GameScene::Update()
{
	player->Update(Input::Get());
	stage->Update();

	/*if (Input::IsKeyTrigger(DIK_SPACE))
	{
		sceneChenger->SceneChenge(SceneChenger::Scene::Title, true);
	}*/
	if (Input::IsKeyTrigger(DIK_R))
	{
		player->Reset();
	}
}

void GameScene::Draw()
{
	DirectXInit* w = DirectXInit::GetInstance();

	w->ClearScreen();
	draw.SetDrawBlendMode(BLENDMODE_ALPHA);
	ShaderManager::blendMode = ShaderManager::BlendMode::ALPHA;

	// 背景
	for (int y = 0; y * 128 < w->windowHeight; y++)
	{
		for (int x = 0; x * 128 < w->windowWidth; x++)
		{
			draw.DrawTextrue(
				x * 128.0f,
				y * 128.0f,
				128.0f,
				128.0f,
				0.0f,
				background,
				DirectX::XMFLOAT2(0.0f, 0.0f)
			);
		}
	}

	// 3Dオブジェクト
	stage->Draw();
	player->Draw();

	// 前景
	draw.DrawTextrue(0, 0, 144.0f, 32.0f, 0.0f, 0, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	draw.DrawString(0, 0, 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "WASD:MOVE");

	w->ScreenFlip();

	// ループの終了処理
	draw.PolygonLoopEnd();
}

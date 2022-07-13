#include "GameScene.h"
#include "./Header/DirectXInit.h"
#include "./Header/Input.h"
#include "Player.h"
#include "./Stage/Stage.h"

#include "./Header/Error.h"

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
}

void GameScene::Update()
{
	player->Update();
	stage->Update();

	/*if (Input::IsKeyTrigger(DIK_SPACE))
	{
		sceneChenger->SceneChenge(SceneChenger::Scene::Title, true);
	}
	if (Input::IsKeyTrigger(DIK_R))
	{
		sceneChenger->SceneChenge(SceneChenger::Scene::Setting, false);
	}*/
}

void GameScene::Draw()
{
	DirectXInit* w = DirectXInit::GetInstance();

	w->ClearScreen();
	draw.SetDrawBlendMode(BLENDMODE_ALPHA);

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
	stage->Draw(200, 200);
	player->Draw(200, 200);

	// 前景
	draw.DrawTextrue(0, 0, 144.0f, 32.0f, 0.0f, 0, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	draw.DrawString(0, 0, 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "WASD:MOVE");

	w->ScreenFlip();

	// ループの終了処理
	draw.PolygonLoopEnd();
}

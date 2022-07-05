#include "GameScene.h"
#include "./Header/DirectXInit.h"
#include "./Header/Input.h"
#include "./Stage/Stage.h"

#include "./Header/Error.h"

namespace
{
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
	stage->Init(&draw);
	stage->LoadStage("./Resources/Game/Stage/test.csv");
}

void GameScene::Update()
{
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
	draw.DrawTextrue(
		w->windowWidth / 2.0f,
		w->windowHeight / 2.0f,
		static_cast<float>(w->windowWidth),
		static_cast<float>(w->windowHeight),
		0.0f,
		background
	);

	// 3Dオブジェクト
	stage->Draw(200, 200);

	// 前景

	w->ScreenFlip();

	// ループの終了処理
	draw.PolygonLoopEnd();
}

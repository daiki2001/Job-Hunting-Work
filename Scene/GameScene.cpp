#include "GameScene.h"
#include "./Header/DirectXInit.h"
#include "./Header/Input.h"

#include "./Header/Error.h"

namespace
{
static const std::wstring backgroundFileName = L"background.png";
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
	background = draw.LoadTextrue((gameResourcesDir + backgroundFileName).c_str());
}

void GameScene::Update()
{
	if (Input::IsKeyTrigger(DIK_SPACE))
	{
		sceneChenger->SceneChenge(SceneChenger::Scene::Title, true);
	}
	if (Input::IsKeyTrigger(DIK_R))
	{
		sceneChenger->SceneChenge(SceneChenger::Scene::Setting, false);
	}
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

	// 前景

	w->ScreenFlip();

	// ループの終了処理
	draw.PolygonLoopEnd();
}

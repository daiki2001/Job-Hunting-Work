#include "TitleScene.h"
#include "./Header/DirectXInit.h"
#include "./Header/Input.h"

#include "./Header/Error.h"

namespace
{
static const std::wstring backgroundFileName = L"background.png";
}

const std::wstring TitleScene::titleResourcesDir = L"./Resources/Title/";

TitleScene::TitleScene(SceneChenger* sceneChenger) :
	BaseScene(sceneChenger),
	background(Engine::FUNCTION_ERROR)
{
	Init();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	background = draw.LoadTextrue((titleResourcesDir + backgroundFileName).c_str());
}

void TitleScene::Update()
{
	if (Input::IsKeyTrigger(DIK_SPACE))
	{
		sceneChenger->SceneChenge(SceneChenger::Scene::Game, true);
	}
}

void TitleScene::Draw()
{
	DirectXInit* w = DirectXInit::GetInstance();

	w->ClearScreen();
	draw.SetDrawBlendMode(BLENDMODE_ALPHA);

	// 背景
	DirectDrawing::ChangeSpriteShader();
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

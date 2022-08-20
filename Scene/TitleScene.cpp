#include "TitleScene.h"
#include "./Header/DirectXInit.h"
#include "InputManager.h"

#include "./Header/Error.h"

namespace
{
static const std::wstring backgroundFileName = L"background.png";
}

const std::wstring TitleScene::titleResourcesDir = L"./Resources/Title/";

TitleScene::TitleScene(DrawPolygon* draw, SceneChenger* sceneChenger) :
	BaseScene(draw, sceneChenger),
	background(Engine::FUNCTION_ERROR)
{
	Init();
}

TitleScene::~TitleScene()
{
#ifdef _DEBUG
	std::string msg = typeid(*this).name() + std::string("から抜けました。\n");
	OutputDebugStringA(msg.c_str());
#endif // _DEBUG
}

void TitleScene::Init()
{
	background = draw->LoadTextrue((titleResourcesDir + backgroundFileName).c_str());
}

void TitleScene::Update()
{
	if (InputManager::Get()->DecisionTrigger())
	{
		sceneChenger->SceneChenge(SceneChenger::Scene::Game, true);
	}
}

void TitleScene::Draw()
{
	DirectXInit* w = DirectXInit::GetInstance();

	w->ClearScreen();
	draw->SetDrawBlendMode(BLENDMODE_ALPHA);

	// 背景
	DirectDrawing::ChangeSpriteShader();
	draw->DrawTextrue(
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
	draw->PolygonLoopEnd();
}

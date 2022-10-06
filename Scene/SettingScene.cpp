#include "SettingScene.h"
#include "./Header/DirectXInit.h"
#include "./Header/Input.h"

#include "./Header/Error.h"

namespace
{
static const std::wstring backgroundFileName = L"background.png";
}

const std::wstring SettingScene::settingResourcesDir = L"./Resources/Setting/";

SettingScene::SettingScene(DrawPolygon* draw, SceneChenger* sceneChenger) :
	BaseScene(draw, sceneChenger),
	background(Engine::FUNCTION_ERROR)
{
	Init();
}

SettingScene::~SettingScene()
{
#ifdef _DEBUG
	std::string msg = typeid(*this).name() + std::string("から抜けました。\n");
	OutputDebugStringA(msg.c_str());
#endif // _DEBUG
}

void SettingScene::Init()
{
	background = draw->LoadTextrue((settingResourcesDir + backgroundFileName).c_str());
}

void SettingScene::Update()
{
	if (Input::IsKeyTrigger(DIK_R))
	{
		sceneChenger->PopScene();
	}
	if (Input::IsKeyTrigger(DIK_SPACE))
	{
		sceneChenger->SceneChenge(SceneChenger::Scene::Title, true);
	}
}

void SettingScene::Draw()
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

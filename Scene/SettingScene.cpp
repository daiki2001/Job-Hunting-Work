#include "SettingScene.h"
#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/EngineGeneral.h"
#include "./Input/Input.h"

namespace
{
static const std::wstring settingResourcesDir = L"./Resources/Setting/";
static const std::wstring backgroundFileName = L"background.png";
}

SettingScene::SettingScene(SceneChanger* sceneChanger) :
	BaseScene(sceneChanger),
	background(FUNCTION_ERROR)
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
	background = Library::DrawPolygon::GetInstance()->LoadTextrue((settingResourcesDir + backgroundFileName).c_str());
}

void SettingScene::Update()
{
	if (Input::IsKeyTrigger(DIK_R))
	{
		sceneChanger->PopScene();
	}
	if (Input::IsKeyTrigger(DIK_SPACE))
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

void SettingScene::Draw()
{
	DirectXInit* w = DirectXInit::GetInstance();
	int winW = w->windowWidth;
	int winH = w->windowHeight;

	Library::DrawPolygon::GetInstance()->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);

	// 3Dオブジェクト

	// 前景
}

void SettingScene::BGDraw()
{
	DirectXInit* w = DirectXInit::GetInstance();
	int winW = w->windowWidth;
	int winH = w->windowHeight;
	auto draw = Library::DrawPolygon::GetInstance();

	draw->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);
	DirectDrawing::ChangeSpriteShader();

	draw->DrawTextrue(
		winW / 2.0f,
		winH / 2.0f,
		static_cast<float>(winW),
		static_cast<float>(winH),
		0.0f,
		background
	);
}

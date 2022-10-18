#include "TitleScene.h"
#include "./Header/DirectXInit.h"
#include "InputManager.h"
#include "./Header/Parameter.h"

namespace
{
static const std::wstring backgroundFileName = L"background.png";
}

const std::wstring TitleScene::titleResourcesDir = L"./Resources/Title/";

TitleScene::TitleScene(DrawPolygon* draw, SceneChenger* sceneChenger) :
	BaseScene(draw, sceneChenger),
	buttonIndex(ButtonIndex::START),
	background(Engine::FUNCTION_ERROR),
	buttonBack(Engine::FUNCTION_ERROR),
	start(Engine::FUNCTION_ERROR),
	editer(Engine::FUNCTION_ERROR)
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
	if (background == FUNCTION_ERROR)
	{
		background = draw->LoadTextrue((titleResourcesDir + backgroundFileName).c_str());
	}
	if (buttonBack == FUNCTION_ERROR)
	{
		buttonBack = Parameter::Get("white1x1");
	}
	if (start == FUNCTION_ERROR)
	{
		start = draw->LoadTextrue((titleResourcesDir + L"Start.png").c_str());
	}
	if (editer == FUNCTION_ERROR)
	{
		editer = draw->LoadTextrue((titleResourcesDir + L"Editer.png").c_str());
	}
}

void TitleScene::Update()
{
	if (InputManager::Get()->MainUpTrigger() || InputManager::Get()->SubUpTrigger())
	{
		buttonIndex = ButtonIndex::START;
	}
	else if (InputManager::Get()->MainDownTrigger() || InputManager::Get()->SubDownTrigger())
	{
		buttonIndex = ButtonIndex::EDITER;
	}

	if (InputManager::Get()->DecisionTrigger())
	{
		switch (buttonIndex)
		{
		case ButtonIndex::START:
		{
			sceneChenger->SceneChenge(SceneChenger::Scene::Game, true);
			break;
		}
		case ButtonIndex::EDITER:
		{
			sceneChenger->SceneChenge(SceneChenger::Scene::StageEditer, true);
			break;
		}
		default:
			break;
		}
	}
}

void TitleScene::Draw()
{
	DirectXInit* w = DirectXInit::GetInstance();

	w->ClearScreen();
	draw->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);

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
	DirectDrawing::ChangeSpriteShader();
	switch (buttonIndex)
	{
	case ButtonIndex::START:
	{
		draw->DrawTextrue(
			w->windowWidth / 2.0f,
			w->windowHeight / 2.0f + 150.0f,
			352.0f,
			64.0f,
			0.0f,
			buttonBack,
			{ 0.5f, 0.5f },
			{ 0.0f, 0.0f, 0.0f, 0.5f }
		);
		break;
	}
	case ButtonIndex::EDITER:
	{
		draw->DrawTextrue(
			w->windowWidth / 2.0f,
			w->windowHeight / 2.0f + 64.0f + 150.0f + 50.0f,
			352.0f,
			64.0f,
			0.0f,
			buttonBack,
			{ 0.5f, 0.5f },
			{ 0.0f, 0.0f, 0.0f, 0.5f }
		);
		break;
	}
	default:
		break;
	}
	draw->DrawTextrue(
		w->windowWidth / 2.0f,
		w->windowHeight / 2.0f + 150.0f,
		144.0f,
		64.0f,
		0.0f,
		start
	);
	draw->DrawTextrue(
		w->windowWidth / 2.0f,
		w->windowHeight / 2.0f + 64.0f + 150.0f + 50.0f,
		352.0f,
		64.0f,
		0.0f,
		editer
	);

	w->ScreenFlip();

	// ループの終了処理
	draw->PolygonLoopEnd();
}

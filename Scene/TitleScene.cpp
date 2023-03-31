#include "TitleScene.h"
#include "./Header/DirectXInit.h"
#include "./Input//GameInput.h"
#include "./Header/Parameter.h"
#include "LoadGraph.h"

namespace
{
static const std::wstring backgroundFileName = L"background.png";
}

const std::wstring TitleScene::titleResourcesDir = L"./Resources/Title/";

TitleScene::TitleScene(SceneChanger* sceneChanger) :
	BaseScene(sceneChanger),
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

	//LoadGraph::Get()->Load(draw);
}

void TitleScene::Update()
{
	if (GameInput::Get()->MainUpTrigger() || GameInput::Get()->SubUpTrigger())
	{
		buttonIndex = ButtonIndex::START;
	}
	else if (GameInput::Get()->MainDownTrigger() || GameInput::Get()->SubDownTrigger())
	{
		buttonIndex = ButtonIndex::EDITER;
	}

	if (isSceneDest)
	{
		if (changeAnimation.GetChange())
		{
			sceneChanger->SceneChange(nextScene, true);
		}
	}
	else
	{
		if (GameInput::Get()->DecisionTrigger())
		{
			isSceneDest = true;
			changeAnimation.Start();

			switch (buttonIndex)
			{
			case ButtonIndex::START:
			{
				nextScene = SceneChanger::Scene::StageSelect;
				break;
			}
			case ButtonIndex::EDITER:
			{
				nextScene = SceneChanger::Scene::StageEditer;
				break;
			}
			default:
				break;
			}
		}
	}
}

void TitleScene::Draw()
{
	DirectXInit* w = DirectXInit::GetInstance();
	int winW = w->windowWidth;
	int winH = w->windowHeight;

	// 3Dオブジェクト
	draw->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);
}

void TitleScene::BGDraw()
{
	DirectXInit* w = DirectXInit::GetInstance();
	int winW = w->windowWidth;
	int winH = w->windowHeight;

	// 背景
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

void TitleScene::UIDraw()
{
	DirectXInit* w = DirectXInit::GetInstance();
	int winW = w->windowWidth;
	int winH = w->windowHeight;

	draw->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);
	DirectDrawing::ChangeSpriteShader();
	switch (buttonIndex)
	{
	case ButtonIndex::START:
	{
		draw->DrawTextrue(
			winW / 2.0f,
			winH / 2.0f + 150.0f,
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
			winW / 2.0f,
			winH / 2.0f + 64.0f + 150.0f + 50.0f,
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
		winW / 2.0f,
		winH / 2.0f + 150.0f,
		144.0f,
		64.0f,
		0.0f,
		start
	);
	draw->DrawTextrue(
		winW / 2.0f,
		winH / 2.0f + 64.0f + 150.0f + 50.0f,
		352.0f,
		64.0f,
		0.0f,
		editer
	);
}

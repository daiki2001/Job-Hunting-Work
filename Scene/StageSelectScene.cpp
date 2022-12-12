#include "StageSelectScene.h"
#include "./Header/DirectXInit.h"
#include "./Stage/Stage.h"
#include "./Header/Parameter.h"
#include "LoadGraph.h"

const std::wstring StageSelectScene::resourcesDir = L"./Resources/Select/";
const std::string StageSelectScene::stageDir = resourcesDirectory + "Stage/";
InputManager* StageSelectScene::inputMgr = InputManager::Get();

StageSelectScene::StageSelectScene(SceneChanger* sceneChanger) :
	BaseScene(sceneChanger),
	stagePath{},
	stageIndex(StageStatus::TITLE),
	background(FUNCTION_ERROR),
	cave(FUNCTION_ERROR),
	title(FUNCTION_ERROR),
	stage(FUNCTION_ERROR),
	userStage(FUNCTION_ERROR),
	arrow(FUNCTION_ERROR),
	number(FUNCTION_ERROR)
{
	Init();
}

StageSelectScene::~StageSelectScene()
{
#ifdef _DEBUG
	std::string msg = typeid(*this).name() + std::string("から抜けました。\n");
	OutputDebugStringA(msg.c_str());
#endif // _DEBUG
}

void StageSelectScene::Init()
{
	LoadGraph::Get()->Load(draw);

	if (background == FUNCTION_ERROR)
	{
		background = draw->LoadTextrue((resourcesDir + L"background.png").c_str());
	}
	if (cave == FUNCTION_ERROR)
	{
		cave = draw->CreateOBJModel(WStringToString(resourcesDir + L"Cave.obj").c_str(), WStringToString(resourcesDir).c_str());
	}
	if (title == FUNCTION_ERROR)
	{
		title = draw->LoadTextrue((resourcesDir + L"Title.png").c_str());
	}
	if (stage == FUNCTION_ERROR)
	{
		stage = draw->LoadTextrue((resourcesDir + L"Stage.png").c_str());
	}
	if (userStage == FUNCTION_ERROR)
	{
		userStage = draw->LoadTextrue((resourcesDir + L"UserStage.png").c_str());
	}
	if (arrow == FUNCTION_ERROR)
	{
		arrow = draw->LoadTextrue((resourcesDir + L"Arrow.png").c_str());
	}
	if (number == FUNCTION_ERROR)
	{
		number = draw->LoadTextrue((resourcesDir + L"Number.png").c_str());
	}

	Camera::targetRadius = 10.0f;
	Camera::longitude = Math::DEGREE_F * (-90.0f);
	Camera::latitude = Math::DEGREE_F * (0.0f);

	Camera::pos = { 0.0f, 0.0f, -10.0f };
	Camera::target = { 0.0f, 0.0f, 0.0f };
	Camera::upVec = { 0.0f, 1.0f, 0.0f };
}

void StageSelectScene::Update()
{
	static int loopCount = 0;

	if (inputMgr->MainLeft() || inputMgr->SubLeft())
	{
		if (loopCount % 5 == 0)
		{
			stageIndex--;
			if (stageIndex < StageStatus::USER)
			{
				stageIndex = StageStatus::USER;
			}
		}

		loopCount++;
		loopCount %= 1000;
	}
	else if (inputMgr->MainRight() || inputMgr->SubRight())
	{
		if (loopCount % 5 == 0)
		{
			stageIndex++;
			if (stageIndex > StageStatus::MAX)
			{
				stageIndex = StageStatus::MAX;
			}
		}

		loopCount++;
		loopCount %= 1000;
	}
	else
	{
		loopCount = 0;
	}
	if (isSceneDest)
	{
		if (changeAnimation.GetChange())
		{
			auto* stage = Stage::Get();
			stage->StaticInit(draw);
			stage->LoadStage(stagePath.c_str());

			sceneChanger->SceneChange(nextScene, true);
		}
	}
	else
	{
		if (inputMgr->DecisionTrigger())
		{
			isSceneDest = true;
			changeAnimation.Start();

			if (stageIndex == StageStatus::TITLE)
			{
				nextScene = SceneChanger::Scene::Title;
			}
			else
			{
				std::string extension = ".csv";
				if (stageIndex == StageStatus::USER)
				{
					stagePath = stageDir + "User/" + "aaa" + extension;
				}
				else
				{
					stagePath = stageDir + "stage";
					stagePath += std::to_string(stageIndex);
					stagePath += extension;
				}
				nextScene = SceneChanger::Scene::Game;
			}
		}
	}
}

void StageSelectScene::Draw()
{
	using namespace DirectX;

	int winW = DirectXInit::GetInstance()->windowWidth;
	int winH = DirectXInit::GetInstance()->windowHeight;

	draw->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);

	// 背景
	DirectDrawing::ChangeSpriteShader();
	draw->DrawTextrue(0.0f,
					  0.0f,
					  static_cast<float>(winW),
					  static_cast<float>(winH),
					  0.0f,
					  background,
					  XMFLOAT2(0.0f, 0.0f));
	if (stageIndex == StageStatus::TITLE)
	{
		//draw->DrawTextrue(winW / 2.0f,
		//				  100.0f,
		//				  100.0f,
		//				  404.0f,
		//				  0.0f,
		//				  Parameter::Get("white1x1"),
		//				  XMFLOAT2(0.5f, 0.0f),
		//				  XMFLOAT4(255.0f / 255.0f,
		//						   243.0f / 255.0f,
		//						   146.0f / 255.0f,
		//						   1.0f));
		draw->DrawTextrue(winW / 2.0f,
						  100.0f,
						  336.0f,
						  96.0f,
						  0.0f,
						  title,
						  XMFLOAT2(0.5f, 0.0f));
	}
	else if (stageIndex == StageStatus::USER)
	{
		DirectDrawing::ChangeMaterialShader();
		draw->DrawOBJ(cave,
					  Vector3(0.0f, 0.0f, 30.0f),
					  Math::rotateY(90.0f * Math::DEGREE_F),
					  scale_xyz(7.0f));
		DirectDrawing::ChangeSpriteShader();
		draw->DrawTextrue(winW / 2.0f,
						  100.0f,
						  640.0f,
						  128.0f,
						  0.0f,
						  userStage,
						  XMFLOAT2(0.5f, 0.0f));
	}
	else
	{
		DirectDrawing::ChangeMaterialShader();
		draw->DrawOBJ(cave,
					  Vector3(0.0f, 0.0f, 30.0f),
					  Math::rotateY(90.0f * Math::DEGREE_F),
					  scale_xyz(7.0f));
		DirectDrawing::ChangeSpriteShader();
		draw->DrawTextrue(winW / 2.0f - 70.0f,
						  100.0f,
						  320.0f,
						  128.0f,
						  0.0f,
						  stage,
						  XMFLOAT2(0.5f, 0.0f));
		draw->DrawCutTextrue(winW / 2.0f + 128.0f,
							 100.0f,
							 60.0f,
							 96.0f,
							 XMFLOAT2(60.0f * stageIndex, 0.0f),
							 XMFLOAT2(58.0f, 96.0f),
							 0.0f,
							 number,
							 XMFLOAT2(0.5f, 0.0f));
	}

	// 左右の三角形の描画
	DirectDrawing::ChangeSpriteShader();
	if (stageIndex != StageStatus::MAX)
	{
		draw->DrawTextrue(winW - 96.0f,
						  winH / 2.0f,
						  128.0f,
						  128.0f,
						  0.0f,
						  arrow,
						  XMFLOAT2(0.5f, 0.5f));
	}
	if (stageIndex != StageStatus::USER)
	{
		draw->DrawTextrue(96.0f,
						  winH / 2.0f,
						  128.0f,
						  128.0f,
						  180.0f * Math::DEGREE_F,
						  arrow,
						  XMFLOAT2(0.5f, 0.5f));
	}

	draw->DrawString(winW / 2.0f - (4.0f * 9.0f * 4.0f), winH - 64.0f, 4.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "Press Space");
}

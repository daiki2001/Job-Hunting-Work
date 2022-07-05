#include "GameScene.h"
#include "./Header/DirectXInit.h"
#include "./Header/Input.h"
#include "./Stage/BlockManager.h"

#include "./Header/Error.h"

namespace
{
static BlockManager* blockMgr = BlockManager::Get();
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
	blockMgr->Init(&draw);

	for (size_t y = 0; y < 5; y++)
	{
		for (size_t x = 0; x < 5; x++)
		{
			int index = blockMgr->CreateBlock(BlockManager::TypeId::NONE);

			if (index == Engine::FUNCTION_ERROR)
			{
				continue;
			}

			auto& block = blockMgr->GetBlock(index);
			block.posX = x * BlockType::WIDTH + 200;
			block.posY = y * BlockType::HEIGHT + 200;
		}
	}
}

void GameScene::Update()
{
	blockMgr->Update();

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
	blockMgr->Draw();

	// 前景

	w->ScreenFlip();

	// ループの終了処理
	draw.PolygonLoopEnd();
}

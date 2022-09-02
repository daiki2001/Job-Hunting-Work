#include "StageEditorScene.h"
#include "./Header/DirectXInit.h"
#include "InputManager.h"
#include "./Header/Camera.h"

namespace
{
InputManager* inputMgr = InputManager::Get();
}

const std::wstring StageEditorScene::resourcesDir = L"./Resources/";

StageEditorScene::StageEditorScene(DrawPolygon* draw, SceneChenger* sceneChenger) :
	BaseScene(draw, sceneChenger),
	stage{},
	mapArray{},
	mapIndex(0),
	blockIndex(1),
	background(Engine::FUNCTION_ERROR),
	cursor(Engine::FUNCTION_ERROR)
{
	Init();
	stage.Reset();
}

StageEditorScene::~StageEditorScene()
{
#ifdef _DEBUG
	std::string msg = typeid(*this).name() + std::string("から抜けました。\n");
	OutputDebugStringA(msg.c_str());
#endif // _DEBUG
}

void StageEditorScene::Init()
{
	stage.Init(draw);

	// 背景画像の読み込み
	if (background == FUNCTION_ERROR)
	{
		background = draw->LoadTextrue((resourcesDir + L"./Game/background.png").c_str());
	}
	if (cursor == FUNCTION_ERROR)
	{
		cursor = draw->LoadTextrue((resourcesDir + L"UI/cursor.png").c_str());
	}

	Area::GetBlockManager()->AllDeleteBlock();
	for (int i = 0; i < sizeof(mapArray) / sizeof(mapArray[0]); i++)
	{
		mapArray[i] = BlockManager::TypeId::NONE;
		Area::GetBlockManager()->CreateBlock(BlockManager::TypeId(mapArray[i]));
		Area::GetBlockManager()->GetBlock(i).pos.x = static_cast<float>(i % STAGE_WIDTH) * 1.0f;
		Area::GetBlockManager()->GetBlock(i).pos.y = static_cast<float>(i / STAGE_WIDTH) * -1.0f;
	}

	mapIndex = 0;

	Camera::targetRadius = 10.0f;
	Camera::longitude = Math::DEGREE_F * (-90.0f);
	Camera::latitude = Math::DEGREE_F * (0.0f);

	Camera::pos = { 0.0f, 0.0f, -10.0f };
	Camera::target = { 7.0f, -3.0f, 0.0f };
	Camera::upVec = { 0.0f, 1.0f, 0.0f };
}

void StageEditorScene::Update()
{
	if (Input::IsKeyTrigger(DIK_F1))
	{
		sceneChenger->SceneChenge(SceneChenger::Scene::Title, true);
	}

	if (inputMgr->MainLeftTrigger())
	{
		if (mapIndex % STAGE_WIDTH > 0)
		{
			mapIndex -= 1;
		}
	}
	if (inputMgr->MainRightTrigger())
	{
		if (mapIndex % STAGE_WIDTH < STAGE_WIDTH - 1)
		{
			mapIndex += 1;
		}
	}
	if (inputMgr->MainUpTrigger())
	{
		if (mapIndex / STAGE_WIDTH > 0)
		{
			mapIndex -= STAGE_WIDTH;
		}
	}
	if (inputMgr->MainDownTrigger())
	{
		if (mapIndex / STAGE_WIDTH < STAGE_HEIGHT - 1)
		{
			mapIndex += STAGE_WIDTH;
		}
	}

	if (inputMgr->SubLeftTrigger())
	{
		if (blockIndex > 0)
		{
			blockIndex -= 1;
		}
	}
	if (inputMgr->SubRightTrigger())
	{
		if (blockIndex < BlockManager::TypeId::MAX - 1)
		{
			blockIndex += 1;
		}
	}

	if (inputMgr->DecisionTrigger())
	{
		mapArray[mapIndex] = blockIndex;
	}

	for (int i = 0; i < sizeof(mapArray) / sizeof(mapArray[0]); i++)
	{
		if (BlockManager::TypeId(mapArray[i]) != Area::GetBlockManager()->GetBlock(i).GetTypeId())
		{
			Area::GetBlockManager()->ChengeBlock(i, BlockManager::TypeId(mapArray[i]));
			Area::GetBlockManager()->GetBlock(i).pos.x = static_cast<float>(i % STAGE_WIDTH) * 1.0f;
			Area::GetBlockManager()->GetBlock(i).pos.y = static_cast<float>(i / STAGE_WIDTH) * -1.0f;
		}
	}
}

void StageEditorScene::Draw()
{
	using namespace Engine::Math;

	DirectXInit* w = DirectXInit::GetInstance();
	const Vector3 offset = Vector3(7.0f, -3.0f, 0.0f);

	w->ClearScreen();
	draw->SetDrawBlendMode(BLENDMODE_ALPHA);

	// 背景
	DirectDrawing::ChangeSpriteShader();
	for (int y = 0; y * 128 < w->windowHeight; y++)
	{
		for (int x = 0; x * 128 < w->windowWidth; x++)
		{
			draw->DrawTextrue(
				x * 128.0f,
				y * 128.0f,
				128.0f,
				128.0f,
				0.0f,
				background,
				DirectX::XMFLOAT2(0.0f, 0.0f)
			);
		}
	}

	// 3Dオブジェクト
	stage.Draw();

	// 前景
	DirectDrawing::ChangeSpriteShader();
	draw->DrawTextrue((static_cast<float>(mapIndex % STAGE_WIDTH) - 7.0f) * 64.0f + w->windowWidth / 2.0f,
					  (static_cast<float>(mapIndex / STAGE_WIDTH) - 3.0f) * 64.0f + w->windowHeight / 2.0f,
					  64.0f, 64.0f, 0.0f, cursor);

	draw->DrawString(0.0f, 0.0f, 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "Block:%d", blockIndex);

	w->ScreenFlip();

	// ループの終了処理
	draw->PolygonLoopEnd();
}

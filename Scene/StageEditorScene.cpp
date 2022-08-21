#include "StageEditorScene.h"
#include "./Header/DirectXInit.h"
#include "InputManager.h"
#include "./Header/Camera.h"

namespace
{
InputManager* inputMgr = InputManager::Get();
}

const std::wstring StageEditorScene::resourcesDir = L"./Resources/";
BlockManager* StageEditorScene::block_mgr = BlockManager::Get();
int StageEditorScene::wall_obj = FUNCTION_ERROR;
int StageEditorScene::door_obj = FUNCTION_ERROR;

StageEditorScene::StageEditorScene(DrawPolygon* draw, SceneChenger* sceneChenger) :
	BaseScene(draw, sceneChenger),
	mapArray{},
	mapIndex(0),
	blockIndex(0),
	background(Engine::FUNCTION_ERROR)
{
	Init();
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
	block_mgr->Init(draw);

	if (background == FUNCTION_ERROR)
	{
		background = draw->LoadTextrue((resourcesDir + L"Game/background.png").c_str());
	}

	// 外壁のモデルの読み込み
	if (wall_obj == FUNCTION_ERROR)
	{
		wall_obj = draw->CreateOBJModel("./Resources/Game/Wall.obj", "./Resources/Game/");
	}
	if (door_obj == FUNCTION_ERROR)
	{
		door_obj = draw->Create3Dbox(1.0f, 1.0f, 1.0f);
	}

	block_mgr->AllDeleteBlock();
	for (size_t i = 0; i < sizeof(mapArray) / sizeof(mapArray[0]); i++)
	{
		mapArray[i] = BlockManager::TypeId::NONE;
		block_mgr->CreateBlock(BlockManager::TypeId(mapArray[i]));
		block_mgr->GetBlock(i).pos.x = static_cast<float>(i % STAGE_WIDTH) * 1.0f;
		block_mgr->GetBlock(i).pos.y = static_cast<float>(i / STAGE_WIDTH) * -1.0f;
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

	for (size_t i = 0; i < sizeof(mapArray) / sizeof(mapArray[0]); i++)
	{
		if (BlockManager::TypeId(mapArray[i]) != block_mgr->GetBlock(i).GetTypeId())
		{
			block_mgr->ChengeBlock(i, BlockManager::TypeId(mapArray[i]));
			block_mgr->GetBlock(i).pos.x = static_cast<float>(i % STAGE_WIDTH) * 1.0f;
			block_mgr->GetBlock(i).pos.y = static_cast<float>(i / STAGE_WIDTH) * -1.0f;
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
	// 外壁の描画
	DirectDrawing::ChangeMaterialShader();
	draw->DrawOBJ(wall_obj, Vector3(-7.5f, +3.5f, 0.0f) + offset, Math::rotateZ(0 * Math::DEGREE_F * 90.0f), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(-7.5f, -3.5f, 0.0f) + offset, Math::rotateZ(1 * Math::DEGREE_F * 90.0f), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+7.5f, -3.5f, 0.0f) + offset, Math::rotateZ(2 * Math::DEGREE_F * 90.0f), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+7.5f, +3.5f, 0.0f) + offset, Math::rotateZ(3 * Math::DEGREE_F * 90.0f), Vector3(2.0f, 2.0f, 2.0f));

	if (block_mgr->GetDoor() == false)
	{
		DirectDrawing::ChangeOBJShader();
		draw->Draw(door_obj, Vector3(0.0f, +4.5f, 0.0f) + offset, Math::Identity(), Vector3(11.0f, 1.0f, 2.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	block_mgr->Draw();

	// 前景
	DirectDrawing::ChangeSpriteShader();
	//draw->DrawTextrue(0, 0, 144.0f, 32.0f, 0.0f, 0, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	draw->DrawString(0.0f, 0.0f, 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "X:%d, Y:%d", mapIndex % STAGE_WIDTH, mapIndex / STAGE_WIDTH);
	draw->DrawString(0.0f, 32.0f, 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "Block:%d", blockIndex);

	w->ScreenFlip();

	// ループの終了処理
	draw->PolygonLoopEnd();
}

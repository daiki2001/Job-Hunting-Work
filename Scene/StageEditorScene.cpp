#include "StageEditorScene.h"
#include "./Header/DirectXInit.h"
#include "./Header/Camera.h"

const std::wstring StageEditorScene::resourcesDir = L"./Resources/";
InputManager* StageEditorScene::inputMgr = InputManager::Get();
Stage* StageEditorScene::stage = Stage::Get();

StageEditorScene::StageEditorScene(DrawPolygon* draw, SceneChenger* sceneChenger) :
	BaseScene(draw, sceneChenger),
	mapArray{},
	mapIndex(0),
	blockIndex(1),
	cursorState(CursorState::BLOCKS),
	background(Engine::FUNCTION_ERROR),
	cursor(Engine::FUNCTION_ERROR)
{
	Init();
	stage->Reset();
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
	stage->StaticInit(draw);

	// 背景画像の読み込み
	if (background == FUNCTION_ERROR)
	{
		background = draw->LoadTextrue((resourcesDir + L"./Game/background.png").c_str());
	}
	if (cursor == FUNCTION_ERROR)
	{
		cursor = draw->LoadTextrue((resourcesDir + L"UI/cursor.png").c_str());
	}

	auto blockMgr = Stage::GetBlockManager();
	if (blockMgr != nullptr)
	{
		blockMgr->AllDeleteBlock();
		for (int i = 0; i < sizeof(mapArray) / sizeof(mapArray[0]); i++)
		{
			mapArray[i] = BlockManager::TypeId::NONE;
			blockMgr->CreateBlock(BlockManager::TypeId(mapArray[i]));
			blockMgr->GetBlock(i).pos.x = static_cast<float>(i % STAGE_WIDTH) * 1.0f;
			blockMgr->GetBlock(i).pos.y = static_cast<float>(i / STAGE_WIDTH) * -1.0f;
		}
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

	CursorMove();

	if (cursorState == CursorState::BLOCKS)
	{
		SelectBlock();
	}
	else
	{

	}

	if (inputMgr->DecisionTrigger())
	{
		// ブロックの配置
		mapArray[mapIndex] = blockIndex;
	}

	for (int i = 0; i < sizeof(mapArray) / sizeof(mapArray[0]); i++)
	{
		if (static_cast<BlockManager::TypeId>(mapArray[i]) != Stage::GetBlockManager()->GetBlock(i).typeId)
		{
			Stage::GetBlockManager()->ChengeBlock(i, BlockManager::TypeId(mapArray[i]));
			Stage::GetBlockManager()->GetBlock(i).pos.x = static_cast<float>(i % STAGE_WIDTH) * 1.0f;
			Stage::GetBlockManager()->GetBlock(i).pos.y = static_cast<float>(i / STAGE_WIDTH) * -1.0f;
		}
	}
}

void StageEditorScene::Draw()
{
	using namespace Engine::Math;

	DirectXInit* w = DirectXInit::GetInstance();
	const Vector3 offset = Vector3(7.0f, -3.0f, 0.0f);

	w->ClearScreen();
	draw->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);

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
	stage->Draw();

	// 前景
	DirectDrawing::ChangeSpriteShader();
	switch (cursorState)
	{
	case StageEditorScene::BLOCKS:
	{
		draw->DrawTextrue((static_cast<float>(mapIndex % STAGE_WIDTH) - 7.0f) * 64.0f + w->windowWidth / 2.0f,
						  (static_cast<float>(mapIndex / STAGE_WIDTH) - 3.0f) * 64.0f + w->windowHeight / 2.0f,
						  64.0f, 64.0f, 0.0f, cursor);
		break;
	}
	case StageEditorScene::DOOR_UP:
	case StageEditorScene::DOOR_DOWN:
	{
		float isMinus = (cursorState == StageEditorScene::DOOR_DOWN) ? 1.0f : -1.0f;
		draw->DrawTextrue(0.0f + w->windowWidth / 2.0f,
						  (static_cast<float>(mapIndex / STAGE_WIDTH) - 3.0f + isMinus) * 64.0f + w->windowHeight / 2.0f,
						  64.0f * 3.0f, 64.0f, 0.0f, cursor);
		break;
	}
	case StageEditorScene::DOOR_LEFT:
	case StageEditorScene::DOOR_RIGHT:
	{
		float isMinus = (cursorState == StageEditorScene::DOOR_RIGHT) ? 1.0f : -1.0f;
		draw->DrawTextrue((static_cast<float>(mapIndex % STAGE_WIDTH) - 7.0f + isMinus) * 64.0f + w->windowWidth / 2.0f,
						  0.0f + w->windowHeight / 2.0f,
						  64.0f, 64.0f * 3.0f, 0.0f, cursor);
		break;
	}
	default:
		break;
	}

	draw->DrawString(0.0f, 0.0f, 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "Block:%d", blockIndex);

	w->ScreenFlip();

	// ループの終了処理
	draw->PolygonLoopEnd();
}

void StageEditorScene::CursorMove()
{
	if (inputMgr->MainLeftTrigger())
	{
		CursorMoveLeft();
	}
	if (inputMgr->MainRightTrigger())
	{
		CursorMoveRight();
	}
	if (inputMgr->MainUpTrigger())
	{
		CursorMoveUp();
	}
	if (inputMgr->MainDownTrigger())
	{
		CursorMoveDown();
	}
}

void StageEditorScene::SelectBlock()
{
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
}

void StageEditorScene::CursorMoveLeft()
{
	if (cursorState == CursorState::BLOCKS)
	{
		if (mapIndex % STAGE_WIDTH > 0)
		{
			// カーソル移動
			mapIndex -= 1;
		}
		else
		{
			// カーソルを左側のドアに移動
			cursorState = CursorState::DOOR_LEFT;
		}
	}
	else if (cursorState == CursorState::DOOR_LEFT)
	{
		// カーソルを右側のドアに移動(ループ)
		mapIndex += STAGE_WIDTH - 1;
		cursorState = CursorState::DOOR_RIGHT;
	}
	else if (cursorState == CursorState::DOOR_RIGHT)
	{
		// カーソルをブロックに移動
		cursorState = CursorState::BLOCKS;
	}
}

void StageEditorScene::CursorMoveRight()
{
	if (cursorState == CursorState::BLOCKS)
	{
		if (mapIndex % STAGE_WIDTH < STAGE_WIDTH - 1)
		{
			mapIndex += 1;
		}
		else
		{
			// カーソルが右側のドアに移動
			cursorState = CursorState::DOOR_RIGHT;
		}
	}
	else if (cursorState == CursorState::DOOR_RIGHT)
	{
		// カーソルが左側のドアに移動(ループ)
		mapIndex -= STAGE_WIDTH - 1;
		cursorState = CursorState::DOOR_LEFT;
	}
	else if (cursorState == CursorState::DOOR_LEFT)
	{
		// カーソルをブロックに移動
		cursorState = CursorState::BLOCKS;
	}
}

void StageEditorScene::CursorMoveUp()
{
	if (cursorState == CursorState::BLOCKS)
	{
		if (mapIndex / STAGE_WIDTH > 0)
		{
			// カーソル移動
			mapIndex -= STAGE_WIDTH;
		}
		else
		{
			// カーソルが上側のドアに移動
			cursorState = CursorState::DOOR_UP;
		}
	}
	else if (cursorState == CursorState::DOOR_UP)
	{
		// カーソルが下側のドアに移動(ループ)
		mapIndex += STAGE_WIDTH * (STAGE_HEIGHT - 1);
		cursorState = CursorState::DOOR_DOWN;
	}
	else if (cursorState == CursorState::DOOR_DOWN)
	{
		// カーソルをブロックに移動
		cursorState = CursorState::BLOCKS;
	}
}

void StageEditorScene::CursorMoveDown()
{
	if (cursorState == CursorState::BLOCKS)
	{
		if (mapIndex / STAGE_WIDTH < STAGE_HEIGHT - 1)
		{
			// カーソル移動
			mapIndex += STAGE_WIDTH;
		}
		else
		{
			// カーソルが下側のドアに移動
			cursorState = CursorState::DOOR_DOWN;
		}
	}
	else if (cursorState == CursorState::DOOR_DOWN)
	{
		// カーソルが上側のドアに移動(ループ)
		mapIndex -= STAGE_WIDTH * (STAGE_HEIGHT - 1);
		cursorState = CursorState::DOOR_UP;
	}
	else if (cursorState == CursorState::DOOR_UP)
	{
		// カーソルをブロックに移動
		cursorState = CursorState::BLOCKS;
	}
}

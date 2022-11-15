#include "StageEditorScene.h"
#include "./Header/DirectXInit.h"
#include "./Header/Camera.h"
#include "./Header/Parameter.h"
#include "LoadGraph.h"

const std::wstring StageEditorScene::resourcesDir = L"./Resources/";
InputManager* StageEditorScene::inputMgr = InputManager::Get();
Stage* StageEditorScene::stage = Stage::Get();

StageEditorScene::StageEditorScene(SceneChanger* sceneChanger) :
	BaseScene(sceneChanger),
	mapArray{},
	mapIndex(0),
	blockIndex(1),
	doorIndex(1),
	cursorState(CursorState::BLOCKS),
	background(Engine::FUNCTION_ERROR),
	cursor(Engine::FUNCTION_ERROR),
	frame(Engine::FUNCTION_ERROR)
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
	LoadGraph::Get()->Load(draw);
	stage->StaticInit(draw);

	// 背景画像の読み込み
	if (background == FUNCTION_ERROR)
	{
		background = draw->LoadTextrue((resourcesDir + L"./Game/background.png").c_str());
	}
	// カーソル画像の読み込み
	if (cursor == FUNCTION_ERROR)
	{
		cursor = draw->LoadTextrue((resourcesDir + L"./UI/Cursor.png").c_str());
	}
	// フレーム画像の読み込み
	if (frame == FUNCTION_ERROR)
	{
		frame = draw->LoadTextrue((resourcesDir + L"./UI/Frame.png").c_str());
	}

	Stage::AllDeleteRoom();
	Stage::CreateRoom();

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
		isSceneDest = true;
		nextScene = SceneChanger::Scene::Title;
		changeAnimation.Start();
	}

	CursorMove();

	if (cursorState == CursorState::BLOCKS)
	{
		SelectBlock();

		if (inputMgr->DecisionTrigger())
		{
			// ブロックの配置
			Stage::GetBlockManager()->ChengeBlock(mapIndex, BlockManager::TypeId(blockIndex));
		}
	}
	else
	{
		SelectDoor();

		if (inputMgr->DecisionTrigger())
		{
			if (doorIndex == Door::DoorStatus::ROOM_CREATE)
			{
				// 部屋の生成
				int createRoomDir = Stage::CreateRoom(cursorState - 1);
				CursorMove(createRoomDir);
			}
			else
			{
				// ドア・壁の配置
				Stage::SetDoorStatus(static_cast<Door::DoorStatus>(doorIndex), static_cast<Area::DoorNum>(cursorState - 1));
			}
		}
	}

	if (Input::IsControlKey() && Input::IsKeyTrigger(DIK_S))
	{
		static const std::string userStageDir = resourcesDirectory + "Stage/User/";
		stage->WirteStage((userStageDir + "aaa.csv").c_str());
	}

	if (isSceneDest)
	{
		if (changeAnimation.GetChange())
		{
			sceneChanger->SceneChange(nextScene, true);
		}
	}
}

void StageEditorScene::Draw()
{
	using namespace Engine::Math;

	DirectXInit* w = DirectXInit::GetInstance();
	const int winW = w->windowWidth;
	const int winH = w->windowHeight;
	const Vector3 offset = Vector3(7.0f, -3.0f, 0.0f);

	draw->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);

	// 背景
	DirectDrawing::ChangeSpriteShader();
	for (int y = 0; y * 128 < winH; y++)
	{
		for (int x = 0; x * 128 < winW; x++)
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

	// ブロック配置用カーソル
	switch (cursorState)
	{
	case CursorState::BLOCKS:
	{
		draw->DrawTextrue((static_cast<float>(mapIndex % STAGE_WIDTH) - 7.0f) * 64.0f + winW / 2.0f,
						  (static_cast<float>(mapIndex / STAGE_WIDTH) - 3.0f) * 64.0f + winH / 2.0f,
						  64.0f, 64.0f, 0.0f, cursor);
		break;
	}
	case CursorState::DOOR_UP:
	case CursorState::DOOR_DOWN:
	{
		float isMinus = (cursorState == CursorState::DOOR_DOWN) ? 1.0f : -1.0f;
		draw->DrawTextrue(0.0f + winW / 2.0f,
						  (static_cast<float>(mapIndex / STAGE_WIDTH) - 3.0f + isMinus) * 64.0f + winH / 2.0f,
						  64.0f * 3.0f, 64.0f, 0.0f, cursor);
		break;
	}
	case CursorState::DOOR_LEFT:
	case CursorState::DOOR_RIGHT:
	{
		float isMinus = (cursorState == CursorState::DOOR_RIGHT) ? 1.0f : -1.0f;
		draw->DrawTextrue((static_cast<float>(mapIndex % STAGE_WIDTH) - 7.0f + isMinus) * 64.0f + winW / 2.0f,
						  0.0f + winH / 2.0f,
						  64.0f, 64.0f * 3.0f, 0.0f, cursor);
		break;
	}
	default:
		break;
	}

	static const int blank = 5;
	static const int frameSize = 64;

	if (cursorState == CursorState::BLOCKS)
	{
		// ブロック選択
		for (int i = 0; i < BlockManager::TypeId::MAX; i++)
		{
			if (i == blockIndex)
			{
				int a = draw->SetDrawBlendMode(ShaderManager::BlendMode::SUB);
				DirectDrawing::ChangeSpriteShader();
			}
			draw->DrawTextrue((frameSize + blank) * i + (frameSize / 2.0f), frameSize / 2.0f, frameSize, frameSize, 0.0f, frame);
			if (i == blockIndex)
			{
				draw->SetDrawBlendMode(ShaderManager::BlendMode::ALPHA);
				DirectDrawing::ChangeSpriteShader();
			}

			switch (i)
			{
			case BlockManager::TypeId::WALL:
				draw->DrawTextrue((frameSize + blank) * i + (frameSize / 2.0f), frameSize / 2.0f,
								  frameSize * (3.0f / 4.0f), frameSize * (3.0f / 4.0f), 0.0f,
								  Parameter::Get(LoadGraph::WALL_BLOCK.c_str()));
				break;
			case BlockManager::TypeId::GOAL:
				draw->DrawTextrue((frameSize + blank) * i + (frameSize / 2.0f), frameSize / 2.0f,
								  frameSize * (3.0f / 4.0f), frameSize * (3.0f / 4.0f), 0.0f,
								  Parameter::Get(LoadGraph::GOAL.c_str()));
				break;
			case BlockManager::TypeId::SWITCH:
				draw->DrawTextrue((frameSize + blank) * i + (frameSize / 2.0f), frameSize / 2.0f,
								  frameSize * (3.0f / 4.0f), frameSize * (3.0f / 4.0f), 0.0f,
								  Parameter::Get(LoadGraph::SWITCH_UI.c_str()));
				break;
			case BlockManager::TypeId::KEY:
				draw->DrawTextrue((frameSize + blank) * i + (frameSize / 2.0f), frameSize / 2.0f,
								  frameSize * (3.0f / 4.0f), frameSize * (3.0f / 4.0f), 0.0f,
								  Parameter::Get(LoadGraph::KEY.c_str()));
				break;
			case BlockManager::TypeId::BOMB:
				draw->DrawTextrue((frameSize + blank) * i + (frameSize / 2.0f), frameSize / 2.0f,
								  frameSize * (3.0f / 4.0f), frameSize * (3.0f / 4.0f), 0.0f,
								  Parameter::Get(LoadGraph::BOMB.c_str()));
				break;
			default:
				break;
			}
		}
	}
	else
	{
		// ドア・壁選択
		for (int i = 0; i < Door::DoorStatus::MAX; i++)
		{
			if (i == doorIndex)
			{
				int a = draw->SetDrawBlendMode(ShaderManager::BlendMode::SUB);
				DirectDrawing::ChangeSpriteShader();
			}
			draw->DrawTextrue((frameSize + blank) * i + (frameSize / 2.0f), frameSize / 2.0f, frameSize, frameSize, 0.0f, frame);
			if (i == doorIndex)
			{
				draw->SetDrawBlendMode(ShaderManager::BlendMode::ALPHA);
				DirectDrawing::ChangeSpriteShader();
			}

			switch (i)
			{
			case Door::DoorStatus::CLOSE:
				draw->DrawTextrue((frameSize + blank) * i + (frameSize / 2.0f), frameSize / 2.0f,
								  frameSize * (3.0f / 4.0f), frameSize * (3.0f / 4.0f), 0.0f,
								  Parameter::Get("white1x1"));
				break;
			case Door::DoorStatus::WALL:
				draw->DrawTextrue((frameSize + blank) * i + (frameSize / 2.0f), frameSize / 2.0f,
								  frameSize * (3.0f / 4.0f), frameSize * (3.0f / 4.0f), 0.0f,
								  Parameter::Get(LoadGraph::WALL_UI.c_str()));
				break;
			case Door::DoorStatus::KEY_CLOSE:
				draw->DrawTextrue((frameSize + blank) * i + (frameSize / 2.0f), frameSize / 2.0f,
								  frameSize * (3.0f / 4.0f), frameSize * (3.0f / 4.0f), 0.0f,
								  Parameter::Get(LoadGraph::KEY_CLOSE.c_str()));
				break;
			case Door::DoorStatus::BREAK_WALL:
				draw->DrawTextrue((frameSize + blank) * i + (frameSize / 2.0f), frameSize / 2.0f,
								  frameSize * (3.0f / 4.0f), frameSize * (3.0f / 4.0f), 0.0f,
								  Parameter::Get(LoadGraph::BREAK_WALL_UI.c_str()));
				break;
			case Door::DoorStatus::ROOM_CREATE:
				draw->DrawTextrue((frameSize + blank) * i + (frameSize / 2.0f), frameSize / 2.0f,
								  frameSize * (3.0f / 4.0f), frameSize * (3.0f / 4.0f), 0.0f,
								  Parameter::Get(LoadGraph::CREATE_ROOM.c_str()));
				break;
			default:
				break;
			}
		}
	}

	draw->DrawString(0.0f, winH - (32.0f * (4.0f + 1.0f)), 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
					 "Move:WASD");
	draw->DrawString(0.0f, winH - (32.0f * (3.0f + 1.0f)), 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
					 "Select:Arrow");
	draw->DrawString(0.0f, winH - (32.0f * (2.0f + 1.0f)), 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
					 "Decision:Space");
	draw->DrawString(0.0f, winH - (32.0f * (1.0f + 1.0f)), 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
					 "Save:Ctrl + S");
	draw->DrawString(0.0f, winH - (32.0f * (0.0f + 1.0f)), 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
					 "Title:F1");
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
	if (inputMgr->MainDownTrigger() && !Input::IsControlKey())
	{
		CursorMoveDown();
	}
}

void StageEditorScene::CursorMove(int createRoomDir)
{
	switch (createRoomDir)
	{
	case Area::DoorNum::UP:
		mapIndex += STAGE_WIDTH * (STAGE_HEIGHT - 1);
		cursorState = CursorState::DOOR_DOWN;
		break;
	case Area::DoorNum::DOWN:
		mapIndex -= STAGE_WIDTH * (STAGE_HEIGHT - 1);
		cursorState = CursorState::DOOR_UP;
		break;
	case Area::DoorNum::LEFT:
		mapIndex += STAGE_WIDTH - 1;
		cursorState = CursorState::DOOR_RIGHT;
		break;
	case Area::DoorNum::RIGHT:
		mapIndex -= STAGE_WIDTH - 1;
		cursorState = CursorState::DOOR_LEFT;
		break;
	default:
		break;
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

void StageEditorScene::SelectDoor()
{
	if (inputMgr->SubLeftTrigger())
	{
		if (doorIndex > 0)
		{
			doorIndex -= 1;
		}
	}
	if (inputMgr->SubRightTrigger())
	{
		if (doorIndex < Door::DoorStatus::MAX - 1)
		{
			doorIndex += 1;
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
		if (Stage::MoveLeftRoom() != FUNCTION_ERROR)
		{
			// カーソルを右側のドアに移動(ループ)
			mapIndex += STAGE_WIDTH - 1;
			cursorState = CursorState::DOOR_RIGHT;
		}
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
		if (Stage::MoveRightRoom() != FUNCTION_ERROR)
		{
			// カーソルが左側のドアに移動(ループ)
			mapIndex -= STAGE_WIDTH - 1;
			cursorState = CursorState::DOOR_LEFT;
		}
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
		if (Stage::MoveUpRoom() != FUNCTION_ERROR)
		{
			// カーソルが下側のドアに移動(ループ)
			mapIndex += STAGE_WIDTH * (STAGE_HEIGHT - 1);
			cursorState = CursorState::DOOR_DOWN;
		}
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
		if (Stage::MoveDownRoom() != FUNCTION_ERROR)
		{
			// カーソルが上側のドアに移動(ループ)
			mapIndex -= STAGE_WIDTH * (STAGE_HEIGHT - 1);
			cursorState = CursorState::DOOR_UP;
		}
	}
	else if (cursorState == CursorState::DOOR_UP)
	{
		// カーソルをブロックに移動
		cursorState = CursorState::BLOCKS;
	}
}

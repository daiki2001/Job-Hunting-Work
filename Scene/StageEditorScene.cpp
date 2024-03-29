﻿#include "StageEditorScene.h"
#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/Camera.h"
#include "./Input/GameInput.h"
#include "./Header/EngineGeneral.h"
#include "./Header/Parameter.h"
#include "LoadGraph.h"
#include "UI.h"

#include "./UndoRedo/AddBlock.h"
#include "./UndoRedo/AddDoor.h"
#include "./UndoRedo/CreateRoom.h"
#include "./UndoRedo/RouteSetter.h"
#include "./UndoRedo/CreateFloor.h"

const std::wstring StageEditorScene::resourcesDir = L"./Resources/";
const int StageEditorScene::FRAME_SIZE = 64;
const int StageEditorScene::FRAME_BLANK = 5;

Stage* StageEditorScene::stage = Stage::Get();

StageEditorScene::StageEditorScene(SceneChanger* sceneChanger) :
	BaseScene(sceneChanger),
	mapArray{},
	mapIndex(0),
	blockIndex(1),
	doorIndex(1),
	isRoute(false),
	routeIndex(0),
	cursorState(CursorState::BLOCKS),
	frame(FUNCTION_ERROR),
	cross(FUNCTION_ERROR)
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
	auto draw = Library::DrawPolygon::GetInstance();
	stage->StaticInit();

	// フレーム画像の読み込み
	if (frame == FUNCTION_ERROR)
	{
		frame = draw->LoadTextrue((resourcesDir + L"./UI/Frame.png").c_str());
	}
	// ×画像の読み込み
	if (cross == FUNCTION_ERROR)
	{
		cross = draw->LoadTextrue((resourcesDir + L"./UI/Cross.png").c_str());
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
	if (isRoute)
	{
		RouteSetting();

		if (GameInput::Get()->DecisionTrigger())
		{
			// 迷いの森の道の設定
			RouteSetter add = RouteSetter(Stage::GetRoom(), routeIndex);
			redoUndo.AddCommandList<RouteSetter>("Route Setting", add);
		}
	}
	else
	{
		CursorMove();

		if (cursorState == CursorState::BLOCKS)
		{
			SelectBlock();

			if (GameInput::Get()->DecisionTrigger())
			{
				if (static_cast<BlockManager::TypeId>(blockIndex) == BlockManager::TypeId::NONE ||
					static_cast<BlockManager::TypeId>(blockIndex) == BlockManager::TypeId::WHITE_TILE)
				{
					EraseBlock(static_cast<BlockManager::TypeId>(blockIndex));
				}
				else
				{
					if (stage->GetBlockManager()->GetBlock(mapIndex).typeId == BlockManager::TypeId::HOLE)
					{
						if (stage->GetArea(Stage::GetRoom() + Stage::DOWN_FLOOR).isAlive)
						{
							Stage::MoveRoom(Stage::GetRoom() + Stage::DOWN_FLOOR);
						}
						else
						{
							// 今いる部屋の取得
							Math::Vector3 oldRoomPos = Stage::GetRoom();
							// 部屋の生成
							int createRoomDir = Stage::CreateRoom(Stage::DOWN_FLOOR);
							CreateRoom add = CreateRoom(Stage::GetRoom(), oldRoomPos);
							redoUndo.AddCommandList<CreateRoom>("Create Room", add);
							CursorMove(createRoomDir);

							if (stage->GetBlockManager()->GetBlock(mapIndex).typeId == BlockManager::TypeId::DOWN_STAIRS)
							{
								Stage::GetBlockManager()->ChengeBlock(mapIndex, BlockManager::TypeId::DOWN_STAIRS);
							}
						}
					}
					else if (stage->GetBlockManager()->GetBlock(mapIndex).typeId == BlockManager::TypeId::DOWN_STAIRS)
					{
						if (stage->GetArea(Stage::GetRoom() + Stage::DOWN_FLOOR).isAlive)
						{
							Stage::MoveRoom(Stage::GetRoom() + Stage::DOWN_FLOOR);
						}
						else
						{
							// 今いる部屋の取得
							Math::Vector3 oldRoomPos = Stage::GetRoom();
							// 部屋の生成
							int createRoomDir = Stage::CreateRoom(Stage::DOWN_FLOOR);
							CreateFloor add = CreateFloor(mapIndex, Stage::GetRoom(), oldRoomPos, false);
							redoUndo.AddCommandList<CreateFloor>("Create Floor", add);
							CursorMove(createRoomDir);
						}
					}
					else if (stage->GetBlockManager()->GetBlock(mapIndex).typeId == BlockManager::TypeId::UP_STAIRS)
					{
						if (stage->GetArea(Stage::GetRoom() + Stage::UP_FLOOR).isAlive)
						{
							Stage::MoveRoom(Stage::GetRoom() + Stage::UP_FLOOR);
						}
						else
						{
							// 今いる部屋の取得
							Math::Vector3 oldRoomPos = Stage::GetRoom();
							// 部屋の生成
							int createRoomDir = Stage::CreateRoom(Stage::UP_FLOOR);
							CreateFloor add = CreateFloor(mapIndex, Stage::GetRoom(), oldRoomPos, true);
							redoUndo.AddCommandList<CreateFloor>("Create Floor", add);
							CursorMove(createRoomDir);
						}
					}
					else
					{
						// 今配置されているブロックの取得
						auto oldBlock = stage->GetBlockManager()->GetBlock(mapIndex).typeId;
						// ブロックの配置
						AddBlock add = AddBlock(Stage::GetRoom(), mapIndex, BlockManager::TypeId(blockIndex), oldBlock);
						redoUndo.AddCommandList<AddBlock>("Add Block", add);
					}
				}
			}
		}
		else
		{
			SelectDoor();

			if (GameInput::Get()->DecisionTrigger())
			{
				if (doorIndex == Door::DoorStatus::ROOM_CREATE)
				{
					// 今いる部屋の取得
					Math::Vector3 oldRoomPos = Stage::GetRoom();
					// 部屋の生成
					int createRoomDir = Stage::CreateRoom(cursorState - 1);
					CreateRoom add = CreateRoom(Stage::GetRoom(), oldRoomPos);
					redoUndo.AddCommandList<CreateRoom>("Create Room", add);
					CursorMove(createRoomDir);
				}
				else
				{
					// 今配置されているドアの取得
					auto oldDoor = stage->GetDoorStatus(static_cast<Area::DoorNum>(cursorState - 1));
					// ドア・壁の配置
					AddDoor add = AddDoor(Stage::GetRoom(), static_cast<Area::DoorNum>(cursorState - 1),
										  static_cast<Door::DoorStatus>(doorIndex), oldDoor,
										  cursorState);
					redoUndo.AddCommandList<AddDoor>("Add Door", add);
				}
			}
		}
	}

	if (Input::IsControlKey())
	{
		static const std::string userStageDir = resourcesDirectory + "Stage/User/";
		if (GameInput::Get()->MainDownTrigger())
		{
			stage->WirteStage((userStageDir + "aaa.csv").c_str());
		}
		else if (Input::IsKeyTrigger(DIK_L))
		{
			routeIndex = 0;
			stage->LoadStage((userStageDir + "aaa.csv").c_str());
		}
		else if (Input::IsKeyTrigger(DIK_Z))
		{
			redoUndo.UndoCommand();
		}
		else if (Input::IsKeyTrigger(DIK_Y))
		{
			redoUndo.RedoCommand();
		}
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

	Library::DrawPolygon::GetInstance()->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);

	// 3Dオブジェクト
	stage->Draw();
}

void StageEditorScene::UIDraw()
{
	DirectXInit* w = DirectXInit::GetInstance();
	const int winW = w->windowWidth;
	const int winH = w->windowHeight;
	auto draw = Library::DrawPolygon::GetInstance();

	draw->SetDrawBlendMode(DirectDrawing::BlendMode::ALPHA);
	DirectDrawing::ChangeSpriteShader();

	// ブロック配置用カーソル
	switch (cursorState)
	{
	case CursorState::BLOCKS:
	{
		draw->DrawTextrue((static_cast<float>(mapIndex % STAGE_WIDTH) - 7.0f) * 64.0f + winW / 2.0f,
						  (static_cast<float>(mapIndex / STAGE_WIDTH) - 3.0f) * 64.0f + winH / 2.0f,
						  64.0f, 64.0f, 0.0f, Parameter::Get(LoadGraph::CURSOR.c_str()));
		break;
	}
	case CursorState::DOOR_TOP:
	case CursorState::DOOR_BOTTOM:
	{
		float isMinus = (cursorState == CursorState::DOOR_BOTTOM) ? 1.0f : -1.0f;
		draw->DrawTextrue(0.0f + winW / 2.0f,
						  (static_cast<float>(mapIndex / STAGE_WIDTH) - 3.0f + isMinus) * 64.0f + winH / 2.0f,
						  64.0f * 3.0f, 64.0f, 0.0f, Parameter::Get(LoadGraph::CURSOR.c_str()));
		break;
	}
	case CursorState::DOOR_LEFT:
	case CursorState::DOOR_RIGHT:
	{
		float isMinus = (cursorState == CursorState::DOOR_RIGHT) ? 1.0f : -1.0f;
		draw->DrawTextrue((static_cast<float>(mapIndex % STAGE_WIDTH) - 7.0f + isMinus) * 64.0f + winW / 2.0f,
						  0.0f + winH / 2.0f,
						  64.0f, 64.0f * 3.0f, 0.0f, Parameter::Get(LoadGraph::CURSOR.c_str()));
		break;
	}
	default:
		break;
	}

	bool isSelect = false;

	if (cursorState == CursorState::BLOCKS)
	{
		DrawSelectBlockUI();
	}
	else
	{
		DrawSelectDoorUI();
	}

	for (int i = 0; static_cast<int>(i) < stage->GetArea().GetRoute().size() - 1; i++)
	{
		float angle = 0.0f;
		int tex = 0;

		isSelect = isRoute && (i == routeIndex);

		if (stage->GetArea().GetRoute()[i] == Area::NONE_LOST_FOREST)
		{
			tex = cross;
		}
		else
		{
			switch (stage->GetArea().GetRoute()[i] % 4)
			{
			case Area::DoorNum::TOP:
				angle = (90.0f * 3) * Math::DEGREE_F;
				break;
			case Area::DoorNum::BOTTOM:
				angle = (90.0f * 1) * Math::DEGREE_F;
				break;
			case Area::DoorNum::LEFT:
				angle = Math::PI_F;
				break;
			case Area::DoorNum::RIGHT:
				angle = 0.0f;
				break;
			}

			tex = Parameter::Get(LoadGraph::TRIANGLE.c_str());
		}

		DrawUIBox(winW - FRAME_SIZE / 2.0f,
				  (FRAME_SIZE + FRAME_BLANK) * i + (FRAME_SIZE / 2.0f),
				  FRAME_SIZE,
				  angle,
				  tex,
				  Color::AddAlphaValue(Color::WHITE, 1.0f),
				  isSelect);

		if (stage->GetArea().GetRoute()[i] == Area::NONE_LOST_FOREST)
		{
			break;
		}
	}
	float TEX_SIZE = 64.0f;
	float SCALE = 0.75f;
	if (isRoute == false)
	{
		UI::Get()->WASD(winW - TEX_SIZE * SCALE * 9.0f, winH - TEX_SIZE * SCALE * 2.0f, SCALE);
	}
	UI::Get()->Arrow(winW - TEX_SIZE * SCALE * 3.0f, winH - TEX_SIZE * SCALE * 2.0f, SCALE);
	UI::Get()->Space(winW - TEX_SIZE * SCALE * 5.5f, winH - TEX_SIZE * SCALE * 2.0f, SCALE);

	draw->DrawString(0.0f, winH - (32.0f * (4.0f + 1.0f)), 2.0f,
					 Color::AddAlphaValue(Color::WHITE, 1.0f), Color::AddAlphaValue(Color::BLACK, 0.5f),
					 "Save:Ctrl + S");
	draw->DrawString(0.0f, winH - (32.0f * (3.0f + 1.0f)), 2.0f,
					 Color::AddAlphaValue(Color::WHITE, 1.0f), Color::AddAlphaValue(Color::BLACK, 0.5f),
					 "Load:Ctrl + L");
	draw->DrawString(0.0f, winH - (32.0f * (2.0f + 1.0f)), 2.0f,
					 Color::AddAlphaValue(Color::WHITE, 1.0f), Color::AddAlphaValue(Color::BLACK, 0.5f),
					 "Redo:Ctrl + Z");
	draw->DrawString(0.0f, winH - (32.0f * (1.0f + 1.0f)), 2.0f,
					 Color::AddAlphaValue(Color::WHITE, 1.0f), Color::AddAlphaValue(Color::BLACK, 0.5f),
					 "Undo:Ctrl + Y");
	draw->DrawString(0.0f, winH - (32.0f * (0.0f + 1.0f)), 2.0f,
					 Color::AddAlphaValue(Color::WHITE, 1.0f), Color::AddAlphaValue(Color::BLACK, 0.5f),
					 "Title:F1");
}

void StageEditorScene::EraseBlock(BlockManager::TypeId index)
{
	// 今配置されているブロックの取得
	auto oldBlock = stage->GetBlockManager()->GetBlock(mapIndex).typeId;
	// ブロックの配置
	AddBlock erase = AddBlock(Stage::GetRoom(), mapIndex, index, oldBlock);
	redoUndo.AddCommandList("Delete Block", erase);
}

void StageEditorScene::CursorMove()
{
	if (GameInput::Get()->MainLeftTrigger())
	{
		CursorMoveLeft();
	}
	if (GameInput::Get()->MainRightTrigger())
	{
		CursorMoveRight();
	}
	if (GameInput::Get()->MainUpTrigger())
	{
		CursorMoveUp();
	}
	if (GameInput::Get()->MainDownTrigger() && !Input::IsControlKey())
	{
		CursorMoveDown();
	}
}

void StageEditorScene::CursorMove(int createRoomDir)
{
	switch (createRoomDir)
	{
	case Area::DoorNum::TOP:
		mapIndex += STAGE_WIDTH * (STAGE_HEIGHT - 1);
		cursorState = CursorState::DOOR_BOTTOM;
		break;
	case Area::DoorNum::BOTTOM:
		mapIndex -= STAGE_WIDTH * (STAGE_HEIGHT - 1);
		cursorState = CursorState::DOOR_TOP;
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
	if (GameInput::Get()->SubLeftTrigger())
	{
		if (blockIndex > 0)
		{
			blockIndex -= 1;
		}
	}
	if (GameInput::Get()->SubRightTrigger())
	{
		if (blockIndex < BlockManager::TypeId::MAX - 1)
		{
			blockIndex += 1;
		}
		else
		{
			isRoute = true;
		}
	}
}

void StageEditorScene::SelectDoor()
{
	if (GameInput::Get()->SubLeftTrigger())
	{
		if (doorIndex > 0)
		{
			doorIndex -= 1;
		}
	}
	if (GameInput::Get()->SubRightTrigger())
	{
		if (doorIndex < Door::DoorStatus::MAX - 1)
		{
			doorIndex += 1;
		}
		else
		{
			isRoute = true;
		}
	}
}

void StageEditorScene::RouteSetting()
{
	if (GameInput::Get()->SubUpTrigger())
	{
		if (routeIndex > 0)
		{
			routeIndex -= 1;
		}
	}
	if (GameInput::Get()->SubDownTrigger())
	{
		int r = stage->GetArea().GetRoute(routeIndex);
		if (routeIndex < Area::MAX_COURSE_NUM - 1 &&
			(r != FUNCTION_ERROR &&
			 r != Area::NONE_LOST_FOREST))
		{
			routeIndex += 1;
		}
	}
	if (GameInput::Get()->SubLeftTrigger())
	{
		isRoute = false;
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
			cursorState = CursorState::DOOR_TOP;
		}
	}
	else if (cursorState == CursorState::DOOR_TOP)
	{
		if (Stage::MoveFrontRoom() != FUNCTION_ERROR)
		{
			// カーソルが下側のドアに移動(ループ)
			mapIndex += STAGE_WIDTH * (STAGE_HEIGHT - 1);
			cursorState = CursorState::DOOR_BOTTOM;
		}
	}
	else if (cursorState == CursorState::DOOR_BOTTOM)
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
			cursorState = CursorState::DOOR_BOTTOM;
		}
	}
	else if (cursorState == CursorState::DOOR_BOTTOM)
	{
		if (Stage::MoveBackRoom() != FUNCTION_ERROR)
		{
			// カーソルが上側のドアに移動(ループ)
			mapIndex -= STAGE_WIDTH * (STAGE_HEIGHT - 1);
			cursorState = CursorState::DOOR_TOP;
		}
	}
	else if (cursorState == CursorState::DOOR_TOP)
	{
		// カーソルをブロックに移動
		cursorState = CursorState::BLOCKS;
	}
}

void StageEditorScene::DrawUIBox(float posX, float posY, float size, float angle, int graphHandle, const XMFLOAT4& color, bool flag)
{
	auto draw = Library::DrawPolygon::GetInstance();

	if (flag)
	{
		draw->SetDrawBlendMode(ShaderManager::BlendMode::SUB);
		DirectDrawing::ChangeSpriteShader();
	}
	draw->DrawTextrue(posX, posY, size, size, 0.0f, frame);
	if (flag)
	{
		draw->SetDrawBlendMode(ShaderManager::BlendMode::ALPHA);
		DirectDrawing::ChangeSpriteShader();
	}

	draw->DrawTextrue(posX, posY, size * (3.0f / 4.0f), size * (3.0f / 4.0f), angle, graphHandle, DirectX::XMFLOAT2(0.5f, 0.5f), color);
}

void StageEditorScene::DrawSelectBlockUI(float offsetX, float offsetY)
{
	int graphHandle = FUNCTION_ERROR;
	DirectX::XMFLOAT3 color = Color::WHITE;
	float alpha = 1.0f;
	bool isSelect = false;

	for (int i = 0; i < BlockManager::TypeId::MAX; i++)
	{
		color = Color::WHITE;
		alpha = 1.0f;
		isSelect = (isRoute == false) && (i == blockIndex);

		switch (i)
		{
		case BlockManager::TypeId::NONE:
			graphHandle = cross;
			break;
		case BlockManager::TypeId::SWITCH_BLOCK:
			color = Color::RED;
			goto WALL_GRAPH;
		case BlockManager::TypeId::NOT_SWITCH_BLOCK:
			color = Color::BLUE;
			goto WALL_GRAPH;
		case BlockManager::TypeId::MOVE_BLOCK:
			color = Color::YELLOW;
		case BlockManager::TypeId::WALL:
		WALL_GRAPH:
			graphHandle = Parameter::Get(LoadGraph::WALL_BLOCK.c_str());
			break;
		case BlockManager::TypeId::GOAL:
			graphHandle = Parameter::Get(LoadGraph::GOAL.c_str());
			break;
		case BlockManager::TypeId::SWITCH:
			graphHandle = Parameter::Get(LoadGraph::SWITCH_UI.c_str());
			break;
		case BlockManager::TypeId::KEY:
			graphHandle = Parameter::Get(LoadGraph::KEY.c_str());
			break;
		case BlockManager::TypeId::BOMB:
			graphHandle = Parameter::Get(LoadGraph::BOMB.c_str());
			break;
		case BlockManager::TypeId::HOLE:
			graphHandle = Parameter::Get(LoadGraph::ARROW.c_str());
			break;
		case BlockManager::TypeId::UP_STAIRS:
			graphHandle = Parameter::Get(LoadGraph::UP_STAIRS.c_str());
			break;
		case BlockManager::TypeId::DOWN_STAIRS:
			graphHandle = Parameter::Get(LoadGraph::DOWN_STAIRS.c_str());
			break;
		case BlockManager::TypeId::WHITE_TILE:
			graphHandle = Parameter::Get(LoadGraph::WHITE_TILE.c_str());
			break;
		case BlockManager::TypeId::TORCH:
			graphHandle = Parameter::Get(LoadGraph::TORCH.c_str());
			break;
		case BlockManager::TypeId::BRACELET:
			graphHandle = Parameter::Get(LoadGraph::BRACELET.c_str());
			break;
		case BlockManager::TypeId::TRANSPARENT_KEY:
			graphHandle = Parameter::Get(LoadGraph::KEY.c_str());
			alpha = 0.5f;
			break;
		default:
			break;
		}

		DrawUIBox(((FRAME_SIZE + FRAME_BLANK) * i + (FRAME_SIZE / 2.0f)) + offsetX,
				  (FRAME_SIZE / 2.0f) + offsetY,
				  FRAME_SIZE,
				  0.0f,
				  graphHandle,
				  Color::AddAlphaValue(color, alpha),
				  isSelect);
	}
}

void StageEditorScene::DrawSelectDoorUI(float offsetX, float offsetY)
{
	int graphHandle = FUNCTION_ERROR;
	bool isSelect = false;

	for (int i = 0; i < Door::DoorStatus::MAX; i++)
	{
		isSelect = (isRoute == false) && (i == doorIndex);

		switch (i)
		{
		case Door::DoorStatus::OPEN:
			graphHandle = cross;
			break;
		case Door::DoorStatus::CLOSE:
			graphHandle = Parameter::Get("white1x1");
			break;
		case Door::DoorStatus::WALL:
			graphHandle = Parameter::Get(LoadGraph::WALL_UI.c_str());
			break;
		case Door::DoorStatus::KEY_CLOSE:
			graphHandle = Parameter::Get(LoadGraph::KEY_CLOSE.c_str());
			break;
		case Door::DoorStatus::BREAK_WALL:
			graphHandle = Parameter::Get(LoadGraph::BREAK_WALL_UI.c_str());
			break;
		case Door::DoorStatus::ROOM_CREATE:
			graphHandle = Parameter::Get(LoadGraph::CREATE_ROOM.c_str());
			break;
		default:
			break;
		}

		DrawUIBox(((FRAME_SIZE + FRAME_BLANK) * i + (FRAME_SIZE / 2.0f)) + offsetX,
				  (FRAME_SIZE / 2.0f) + offsetY,
				  FRAME_SIZE,
				  0.0f,
				  graphHandle,
				  Color::AddAlphaValue(Color::WHITE, 1.0f),
				  isSelect);
	}
}

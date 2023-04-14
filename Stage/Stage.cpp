#include "Stage.h"
#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/Camera.h"
#include "./Header/Parameter.h"

const Math::Vector3 Stage::LEFT_ROOM = { -1.0f, 0.0f, 0.0f };
const Math::Vector3 Stage::RIGHT_ROOM = { 1.0f, 0.0f, 0.0f };
const Math::Vector3 Stage::FRONT_ROOM = { 0.0f, -1.0f, 0.0f };
const Math::Vector3 Stage::BACK_ROOM = { 0.0f, 1.0f, 0.0f };
const Math::Vector3 Stage::UP_FLOOR = { 0.0f, 0.0f, 1.0f };
const Math::Vector3 Stage::DOWN_FLOOR = { 0.0f, 0.0f, -1.0f };
std::map<Math::Vector3, Area> Stage::rooms;
Math::Vector3 Stage::nowRoom = { 0.0f, 0.0f, 0.0f };
Math::Vector3 Stage::oldRoomPos = Stage::nowRoom;
Math::Vector3 Stage::moveDir = { 0.0f, 0.0f, 0.0f };
Scroll Stage::scroll = {};
bool Stage::oldScrollFlag = false;

Stage::Stage()
{
	Init();
}

Stage::~Stage()
{
}

Stage* Stage::Get()
{
	static Stage instance = {};
	return &instance;
}

void Stage::StaticInit()
{
	Area::StaticInit();
}

void Stage::Init()
{
	nowRoom = { 0.0f, 0.0f, 0.0f };
}

void Stage::Update()
{
	rooms[nowRoom].Update();

	if (rooms[nowRoom].GetBlockManager()->IsPlayerMove())
	{
		if (rooms[nowRoom].GetBlockManager()->GetStep() == BlockManager::Step::UP)
		{
			MoveUpFloor();
		}
		else if (rooms[nowRoom].GetBlockManager()->GetStep() == BlockManager::Step::DOWN)
		{
			MoveDownFloor();
		}
	}

	if (scroll.GetFlag() == false && oldScrollFlag)
	{
		const int playerInitPos = rooms[nowRoom].GetBlockManager()->GetPlayerInitPos();
		const int playerInitPosX = playerInitPos % BlockManager::STAGE_WIDTH;
		const int playerInitPosY = playerInitPos / BlockManager::STAGE_WIDTH;

		if (playerInitPosX <= 8.0f && playerInitPosX >= 6.0f)
		{
			if ((playerInitPosY < 0.0f) &&
				(rooms[nowRoom].GetDoorStatus(Area::DoorNum::TOP) == Door::DoorStatus::WALL))
			{
				rooms[nowRoom].GetDoor(Area::DoorNum::TOP).CloseEaseInit();
			}
			else if ((playerInitPosY >= BlockManager::STAGE_HEIGHT - 1.0f) &&
					 (rooms[nowRoom].GetDoorStatus(Area::DoorNum::BOTTOM) == Door::DoorStatus::WALL))
			{
				rooms[nowRoom].GetDoor(Area::DoorNum::BOTTOM).CloseEaseInit();
			}
		}
		else if (playerInitPosY <= 4.0f && playerInitPosY >= 2.0f)
		{
			if ((playerInitPosX < 0.0f) &&
				(rooms[nowRoom].GetDoorStatus(Area::DoorNum::LEFT) == Door::DoorStatus::WALL))
			{
				rooms[nowRoom].GetDoor(Area::DoorNum::LEFT).CloseEaseInit();
			}
			else if ((playerInitPosX >= BlockManager::STAGE_WIDTH - 2.0f) &&
					 (rooms[nowRoom].GetDoorStatus(Area::DoorNum::RIGHT) == Door::DoorStatus::WALL))
			{
				rooms[nowRoom].GetDoor(Area::DoorNum::RIGHT).CloseEaseInit();
			}
		}
	}

	oldScrollFlag = scroll.GetFlag();
	scroll.ScrollUpdate(0.1f);
	if (scroll.GetFlag())
	{
		Vector3 moveCamera = moveDir * scroll.GetTime();
		moveCamera.x *= (Area::WALL_SIZE * 2.0f + BlockManager::STAGE_WIDTH);
		moveCamera.y *= -(Area::WALL_SIZE * 2.0f + BlockManager::STAGE_HEIGHT);
		Camera::target = Area::INIT_CAMERA + moveCamera;
	}
	else
	{
		Camera::target = Area::INIT_CAMERA;
	}
}

void Stage::Draw(int offsetX, int offsetY)
{
	int winW = DirectXInit::GetInstance()->windowWidth;
	int winH = DirectXInit::GetInstance()->windowHeight;

	// 下の階層を暗めに描画する
	if (rooms.find(nowRoom + DOWN_FLOOR) != rooms.end() &&
		rooms[nowRoom + DOWN_FLOOR].isAlive)
	{
		rooms[nowRoom + DOWN_FLOOR].Draw({ static_cast<float>(offsetX), static_cast<float>(offsetY), Area::WALL_SIZE });
	}
	DirectDrawing::ChangeSpriteShader();
	Library::DrawPolygon::GetInstance()->DrawTextrue(
		0.0f, 0.0f, static_cast<float>(winW), static_cast<float>(winH),
		0.0f, Parameter::Get("white1x1"), DirectX::XMFLOAT2(0.0f, 0.0f),
		Color::AddAlphaValue(Color::BLACK, 0.5f));

	rooms[nowRoom].Draw({ static_cast<float>(offsetX), static_cast<float>(offsetY), 0.0f });

	MiniMap(DirectXInit::GetInstance()->windowWidth + offsetX, offsetY, 20.0f);
}

void Stage::Reset()
{
	nowRoom = { 0.0f, 0.0f, 0.0f };

	for (auto& i : rooms)
	{
		i.second.Reset();
	}
}

void Stage::ScrollDraw(int offsetX, int offsetY)
{
	static const Vector3 DOWN_FLOOR_POS = { 0.0f, 0.0f, Area::WALL_SIZE };

	int winW = DirectXInit::GetInstance()->windowWidth;
	int winH = DirectXInit::GetInstance()->windowHeight;
	DirectX::XMFLOAT2 scr = {
		moveDir.x * scroll.GetTime(),
		moveDir.y * scroll.GetTime()
	};
	Vector3 offset = { static_cast<float>(offsetX), static_cast<float>(offsetY), 0.0f };

	if (rooms.find(oldRoomPos) == rooms.end())
	{
		oldRoomPos = nowRoom;
	}

	// 下の階層を暗めに描画する
	if (rooms.find(nowRoom + DOWN_FLOOR) != rooms.end() &&
		rooms[nowRoom + DOWN_FLOOR].isAlive)
	{
		rooms[nowRoom + DOWN_FLOOR].Draw(Vector3(
			(static_cast<float>(BlockManager::STAGE_WIDTH) + Area::WALL_SIZE * 2.0f) * moveDir.x,
			(static_cast<float>(BlockManager::STAGE_HEIGHT) + Area::WALL_SIZE * 2.0f) * moveDir.y * -1.0f,
			0.0f) + DOWN_FLOOR_POS + offset);
	}
	if (rooms.find(oldRoomPos + DOWN_FLOOR) != rooms.end() &&
		rooms[oldRoomPos + DOWN_FLOOR].isAlive)
	{
		rooms[oldRoomPos + DOWN_FLOOR].Draw(DOWN_FLOOR_POS + offset);
	}
	DirectDrawing::ChangeSpriteShader();
	Library::DrawPolygon::GetInstance()->DrawTextrue(
		static_cast<float>(winW) / 2.0f, static_cast<float>(winH) / 2.0f,
		static_cast<float>(winW), static_cast<float>(winH),
		0.0f, Parameter::Get("white1x1"), DirectX::XMFLOAT2(0.5f, 0.5f),
		Color::AddAlphaValue(Color::BLACK, 0.5f));

	rooms[nowRoom].Draw(Vector3(
		(static_cast<float>(BlockManager::STAGE_WIDTH) + Area::WALL_SIZE * 2.0f) * moveDir.x,
		(static_cast<float>(BlockManager::STAGE_HEIGHT) + Area::WALL_SIZE * 2.0f) * moveDir.y * -1.0f,
		0.0f) + offset);
	rooms[oldRoomPos].Draw(offset);

	MiniMap(winW + offsetX, offsetY, 20.0f, scr);
}

int Stage::LoadStage(const char* filePath)
{
	if (filePath == nullptr)
	{
		OutputDebugStringA("ファイル名が間違っています。\n");
		return FUNCTION_ERROR;
	}

	rooms.clear();

	FILE* fileHandle;
	errno_t err;
	char string[256] = { 0 };

	size_t roomNum = 0;
	Vector3 roomPos = {};

	err = fopen_s(&fileHandle, filePath, "r");
	if (err != 0)
	{
		OutputDebugStringA("ファイルが開けません。\n");
		return err;
	}

	if (fgets(string, 256, fileHandle) == nullptr)
	{
		OutputDebugStringA("ファイルが読み込めません。\n");
		return FUNCTION_ERROR;
	}

	for (int i = 0; string[i] != '\0'; i++)
	{
		if (string[i] == ',' || string[i] == '\n')
		{
			break;
		}
		else if (string[i] >= '0' && string[i] <= '9')
		{
			roomNum *= 10;
			roomNum += static_cast<size_t>(string[i]) - '0';
		}
	}

	int pos[3] = {};
	for (size_t i = 0; i < roomNum; i++)
	{
		File::LoadMapChip(fileHandle, pos, 3);
		roomPos.x = static_cast<float>(pos[0]);
		roomPos.y = static_cast<float>(pos[1]);
		roomPos.z = static_cast<float>(pos[2]);

		rooms[roomPos].LoadArea(fileHandle);
	}

	fclose(fileHandle);
	return 0;
}

int Stage::WirteStage(const char* filePath)
{
	int roomPos[3] = {};

	FILE* fileHandle;
	File::WriteFileOpen(&fileHandle, filePath);

	size_t deadRoomCount = 0; //描画していない部屋の数
	// 部屋数
	for (auto& i : rooms)
	{
		if (i.second.isAlive == false)
		{
			deadRoomCount++;
		}
	}
	fprintf_s(fileHandle, "%zu\n", rooms.size() - deadRoomCount);

	// 各部屋の情報
	for (auto& i : rooms)
	{
		if (i.second.isAlive == false) continue;

		roomPos[0] = static_cast<int>(i.first.x);
		roomPos[1] = static_cast<int>(i.first.y);
		roomPos[2] = static_cast<int>(i.first.z);

		// 部屋の座標
		File::WriteCSV(fileHandle, roomPos, 3);
		// 部屋の情報
		i.second.WriteArea(fileHandle);
	}

	fclose(fileHandle);
	return 0;
}

int Stage::CreateRoom(int direction)
{
	if (direction < 0 && direction >= 4)
	{
		if (rooms.empty() == false)
		{
			return FUNCTION_ERROR;
		}
	}

	switch (direction)
	{
	case Area::DoorNum::LEFT:
		nowRoom += LEFT_ROOM;
		break;
	case Area::DoorNum::RIGHT:
		nowRoom += RIGHT_ROOM;
		break;
	case Area::DoorNum::TOP:
		nowRoom += FRONT_ROOM;
		break;
	case Area::DoorNum::BOTTOM:
		nowRoom += BACK_ROOM;
		break;
	default:
		break;
	}

	if (rooms.find(nowRoom) != rooms.end())
	{
		rooms[nowRoom].isAlive = true;
		return direction;
	}

	rooms[nowRoom] = Area::GetPlaneArea();
	rooms[nowRoom].isAlive = true;

	return direction;
}

int Stage::CreateRoom(const Vector3& moveRoom)
{
	nowRoom += moveRoom;

	if (rooms.find(nowRoom) != rooms.end())
	{
		rooms[nowRoom].isAlive = true;
		return 0;
	}

	rooms[nowRoom] = Area::GetPlaneArea();
	rooms[nowRoom].isAlive = true;

	return 0;
}

int Stage::DeleteRoom(int direction)
{
	if (direction < 0 && direction >= 4)
	{
		return FUNCTION_ERROR;
	}

	Vector3 moveRoom = {};

	switch (direction)
	{
	case Area::DoorNum::LEFT:
		moveRoom = nowRoom + LEFT_ROOM;
		break;
	case Area::DoorNum::RIGHT:
		moveRoom = nowRoom + RIGHT_ROOM;
		break;
	case Area::DoorNum::TOP:
		moveRoom = nowRoom + FRONT_ROOM;
		break;
	case Area::DoorNum::BOTTOM:
		moveRoom = nowRoom + BACK_ROOM;
		break;
	default:
		break;
	}

	if (rooms.find(moveRoom) == rooms.end())
	{
		return FUNCTION_ERROR;
	}

	rooms[moveRoom].GetBlockManager()->AllDeleteBlock();
	rooms.erase(moveRoom);

	nowRoom = moveRoom;

	return 0;
}

int Stage::DeleteRoom(int direction, const Vector3& roomPos)
{
	if (direction < 0 && direction >= 4 ||
		rooms.find(roomPos) == rooms.end())
	{
		return FUNCTION_ERROR;
	}

	if (roomPos == nowRoom)
	{
		Vector3 moveRoom = {};

		switch (direction)
		{
		case Area::DoorNum::TOP:
			moveRoom = nowRoom + FRONT_ROOM;
			break;
		case Area::DoorNum::BOTTOM:
			moveRoom = nowRoom + BACK_ROOM;
			break;
		case Area::DoorNum::LEFT:
			moveRoom = nowRoom + LEFT_ROOM;
			break;
		case Area::DoorNum::RIGHT:
			moveRoom = nowRoom + RIGHT_ROOM;
			break;
		default:
			break;
		}

		if (rooms.find(moveRoom) == rooms.end())
		{
			return FUNCTION_ERROR;
		}

		rooms[moveRoom].GetBlockManager()->AllDeleteBlock();
		rooms.erase(moveRoom);

		nowRoom = moveRoom;
	}
	else
	{
		rooms[roomPos].GetBlockManager()->AllDeleteBlock();
		rooms.erase(roomPos);
	}

	return 0;
}

void Stage::AllDeleteRoom()
{
	nowRoom = { 0.0f, 0.0f, 0.0f };

	for (auto& i : rooms)
	{
		i.second.GetBlockManager()->AllDeleteBlock();
	}
	rooms.clear();
}

int Stage::MoveRoom(const Vector3& moveRoomPos, const Vector3& direction)
{
	if (rooms.find(moveRoomPos) == rooms.end() ||
		rooms[moveRoomPos].isAlive == false)
	{
		return FUNCTION_ERROR;
	}

	if (direction == Vector3::Zero())
	{
		moveDir = moveRoomPos - nowRoom;
	}
	else
	{
		moveDir = direction.Normalize();
	}

	oldRoomPos = nowRoom;
	nowRoom = moveRoomPos;

	bool isSwitch = false;
	for (size_t i = 0; i < rooms[nowRoom].GetBlockManager()->GetBlockSize(); i++)
	{
		switch (rooms[nowRoom].GetBlockManager()->GetBlock(i).typeId)
		{
		case BlockManager::TypeId::SWITCH:
			isSwitch = true;
			break;
		case BlockManager::TypeId::SWITCH_BLOCK:
			rooms[nowRoom].GetBlockManager()->GetBlock(i).pos.z =
				1.0f * (BlockManager::GetBlockSwitch() == true);
			break;
		case BlockManager::TypeId::NOT_SWITCH_BLOCK:
			rooms[nowRoom].GetBlockManager()->GetBlock(i).pos.z =
				1.0f * (BlockManager::GetBlockSwitch() == false);
			break;
		default:
			break;
		}
	}
	bool isSwitchDoor = false;
	for (size_t i = 0; i < Area::DOOR_COUNT; i++)
	{
		if (rooms[nowRoom].GetDoorStatus(static_cast<Area::DoorNum>(i)) == Door::DoorStatus::CLOSE)
		{
			isSwitchDoor = true;
			break;
		}
	}

	if (isSwitchDoor == false)
	{
		rooms[nowRoom].GetBlockManager()->MapInit();
	}

	oldScrollFlag = scroll.GetFlag();
	scroll.ScrollStart();
	rooms[nowRoom].GetBlockManager()->isInitPlayer = true;
	return 0;
}

int Stage::MoveFrontRoom()
{
	return MoveRoom(nowRoom + FRONT_ROOM);
}

int Stage::MoveBackRoom()
{
	return MoveRoom(nowRoom + BACK_ROOM);
}

int Stage::MoveLeftRoom()
{
	return MoveRoom(nowRoom + LEFT_ROOM);
}

int Stage::MoveRightRoom()
{
	return MoveRoom(nowRoom + RIGHT_ROOM);
}

int Stage::MoveUpFloor()
{
	int resurt = MoveRoom(nowRoom + UP_FLOOR);
	if (resurt != FUNCTION_ERROR)
	{
		rooms[nowRoom].GetBlockManager()->MoveArea();
	}
	return resurt;
}

int Stage::MoveDownFloor()
{
	int resurt = MoveRoom(nowRoom + DOWN_FLOOR);
	if (resurt != FUNCTION_ERROR)
	{
		rooms[nowRoom].GetBlockManager()->MoveArea();
	}
	return resurt;
}

void Stage::LastRoom()
{
	bool isGool = false;

	for (auto& i : rooms)
	{
		auto blockMgr = i.second.GetBlockManager();
		for (size_t j = 0; j < blockMgr->GetBlockSize(); j++)
		{
			if (blockMgr->GetBlock(static_cast<int>(j)).typeId == BlockManager::TypeId::GOAL)
			{
				isGool = true;
				break;
			}
		}

		if (isGool)
		{
			nowRoom = i.first;
			break;
		}
	}
}

void Stage::MiniMap(int offsetX, int offsetY, float scale, DirectX::XMFLOAT2 scroll)
{
	Vector3 offset = {
		offsetX - (1.0f * 3.0f + 2.0f * 3.0f) * scale + 1.0f * scale,
		offsetY + 1.0f * scale + 1.0f * scale,
		0.0f
	};
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(0.625f, 0.625f, 0.625f, 0.75f);
	auto draw = Library::DrawPolygon::GetInstance();

	draw->ChangeSpriteShader();
	for (int i = 0; i < 9; i++)
	{
		float x = static_cast<float>(i % 3);
		float y = static_cast<float>(i / 3);
		Vector3 roomPos = nowRoom + Vector3(x - 1.0f, y - 1.0f, 0.0f);
		if ((rooms.find(roomPos) == rooms.end()) ||
			(rooms[roomPos].isAlive == false))
		{
			continue;
		}

		draw->DrawTextrue(x * (2.0f + 1.0f) * scale + offset.x,
						  y * (2.0f + 1.0f) * scale + offset.y,
						  2.0f * scale,
						  2.0f * scale,
						  0.0f,
						  Parameter::Get("white1x1"),
						  DirectX::XMFLOAT2(0.5f, 0.5f),
						  (roomPos == nowRoom) ? Color::AddAlphaValue(Color::WHITE, color.w) : color);
		if (rooms.find(roomPos + LEFT_ROOM) != rooms.end() &&
			rooms[roomPos].GetDoorStatus(Area::LEFT) == Door::DoorStatus::OPEN &&
			(rooms[roomPos + LEFT_ROOM].isAlive == true))
		{
			if (rooms[roomPos + LEFT_ROOM].GetDoorStatus(Area::RIGHT) == Door::DoorStatus::OPEN)
			{
				if (x == 0.0f)
				{
					draw->DrawTextrue(
						x * (2.0f + 1.0f) * scale - 1.5f * scale + offset.x,
						y * (2.0f + 1.0f) * scale + offset.y,
						1.0f * scale,
						1.0f * scale,
						0.0f,
						Parameter::Get("white1x1"),
						DirectX::XMFLOAT2(0.5f, 0.5f),
						color);
				}
			}
			else
			{
				draw->Draw2DTriangle(
					x * (2.0f + 1.0f) * scale - 1.5f * scale + offset.x,
					y * (2.0f + 1.0f) * scale + offset.y,
					1.0f * scale,
					1.0f * scale,
					0.0f,
					Parameter::Get("white1x1"),
					DirectX::XMFLOAT2(0.5f, 0.5f),
					color);
			}
		}
		if (rooms.find(roomPos + FRONT_ROOM) != rooms.end() &&
			rooms[roomPos].GetDoorStatus(Area::TOP) == Door::DoorStatus::OPEN &&
			(rooms[roomPos + FRONT_ROOM].isAlive == true))
		{
			if (rooms[roomPos + FRONT_ROOM].GetDoorStatus(Area::BOTTOM) == Door::DoorStatus::OPEN)
			{
				if (y == 0.0f)
				{
					draw->DrawTextrue(
						x * (2.0f + 1.0f) * scale + offset.x,
						y * (2.0f + 1.0f) * scale - 1.5f * scale + offset.y,
						1.0f * scale,
						1.0f * scale,
						0.0f,
						Parameter::Get("white1x1"),
						DirectX::XMFLOAT2(0.5f, 0.5f),
						color);
				}
			}
			else
			{
				draw->Draw2DTriangle(
					x * (2.0f + 1.0f) * scale + offset.x,
					y * (2.0f + 1.0f) * scale - 1.5f * scale + offset.y,
					1.0f * scale,
					1.0f * scale,
					90.0f * Math::DEGREE_F,
					Parameter::Get("white1x1"),
					DirectX::XMFLOAT2(0.5f, 0.5f),
					color);
			}
		}
		if (rooms.find(roomPos + RIGHT_ROOM) != rooms.end() &&
			rooms.at(roomPos).GetDoorStatus(Area::RIGHT) == Door::DoorStatus::OPEN &&
			(rooms[roomPos + RIGHT_ROOM].isAlive == true))
		{
			if (rooms[roomPos + RIGHT_ROOM].GetDoorStatus(Area::LEFT) == Door::DoorStatus::OPEN)
			{
				draw->DrawTextrue(
					x * (2.0f + 1.0f) * scale + 1.5f * scale + offset.x,
					y * (2.0f + 1.0f) * scale + offset.y,
					1.0f * scale,
					1.0f * scale,
					0.0f,
					Parameter::Get("white1x1"),
					DirectX::XMFLOAT2(0.5f, 0.5f),
					color);
			}
			else
			{
				draw->Draw2DTriangle(
					x * (2.0f + 1.0f) * scale + 1.5f * scale + offset.x,
					y * (2.0f + 1.0f) * scale + offset.y,
					1.0f * scale,
					1.0f * scale,
					Math::PI_F,
					Parameter::Get("white1x1"),
					DirectX::XMFLOAT2(0.5f, 0.5f),
					color);
			}
		}
		if (rooms.find(roomPos + BACK_ROOM) != rooms.end() &&
			rooms.at(roomPos).GetDoorStatus(Area::BOTTOM) == Door::DoorStatus::OPEN &&
			(rooms[roomPos + BACK_ROOM].isAlive == true))
		{

			if (rooms[roomPos + BACK_ROOM].GetDoorStatus(Area::TOP) == Door::DoorStatus::OPEN)
			{
				draw->DrawTextrue(
					x * (2.0f + 1.0f) * scale + offset.x,
					y * (2.0f + 1.0f) * scale + 1.5f * scale + offset.y,
					1.0f * scale,
					1.0f * scale,
					0.0f,
					Parameter::Get("white1x1"),
					DirectX::XMFLOAT2(0.5f, 0.5f),
					color);
			}
			else
			{
				draw->Draw2DTriangle(
					x * (2.0f + 1.0f) * scale + offset.x,
					y * (2.0f + 1.0f) * scale + 1.5f * scale + offset.y,
					1.0f * scale,
					1.0f * scale,
					270.0f * Math::DEGREE_F,
					Parameter::Get("white1x1"),
					DirectX::XMFLOAT2(0.5f, 0.5f),
					color);
			}
		}
	}
}

const bool Stage::IsGoal()
{
	static bool reslut = false;
	reslut = false;

	for (auto& i : rooms)
	{
		if (i.second.isAlive = true)
		{
			if (i.second.IsGoal())
			{
				Camera::target = Area::INIT_CAMERA;
				reslut = true;
				break;
			}
		}
	}

	return reslut;
}

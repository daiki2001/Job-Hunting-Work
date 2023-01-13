#include "Stage.h"
#include "./Header/DirectXInit.h"
#include "./Header/Parameter.h"

const Math::Vector3 Stage::moveUpRoom = { 0.0f, -1.0f, 0.0f };
const Math::Vector3 Stage::moveDownRoom = { 0.0f, 1.0f, 0.0f };
const Math::Vector3 Stage::moveLeftRoom = { -1.0f, 0.0f, 0.0f };
const Math::Vector3 Stage::moveRightRoom = { 1.0f, 0.0f, 0.0f };
DrawPolygon* Stage::draw = nullptr;
std::map<Math::Vector3, Area> Stage::rooms;
Math::Vector3 Stage::nowRoom = { 0.0f, 0.0f, 0.0f };
Math::Vector3 Stage::oldRoomPos = Stage::nowRoom;
Math::Vector3 Stage::moveDir = { 0.0f, 0.0f, 0.0f };
Scroll Stage::scroll = {};

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

void Stage::StaticInit(DrawPolygon* const draw)
{
	Stage::draw = draw;
	Area::StaticInit(Stage::draw);
}

void Stage::Init()
{
	nowRoom = { 0.0f, 0.0f, 0.0f };
}

void Stage::Update()
{
	rooms[nowRoom].Update();

	scroll.ScrollUpdate(0.1f);
	if (scroll.GetFlag())
	{
		Vector3 moveCamera = moveDir * scroll.GetTime();
		moveCamera = 2.0f * Vector3(
			Area::INIT_CAMERA.x * moveCamera.x,
			Area::INIT_CAMERA.y * moveCamera.y,
			Area::INIT_CAMERA.z * moveCamera.z);
		Camera::target = Area::INIT_CAMERA + moveCamera;
	}
	else
	{
		Camera::target = Area::INIT_CAMERA;
	}
}

void Stage::Draw(int offsetX, int offsetY)
{
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

	rooms[nowRoom].Draw(Vector3(
		(static_cast<float>(Area::STAGE_WIDTH) + Area::WALL_SIZE * 2.0f) * moveDir.x,
		(static_cast<float>(Area::STAGE_HEIGHT) + Area::WALL_SIZE * 2.0f) * moveDir.y * -1.0f,
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
	static int roomPos[3] = {};

	FILE* fileHandle;
	File::WriteFileOpen(&fileHandle, filePath);

	// 部屋数
	fprintf_s(fileHandle, "%zu\n", rooms.size());
	// 各部屋の情報
	for (auto& i : rooms)
	{
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
	case Area::DoorNum::UP:
		nowRoom += moveUpRoom;
		break;
	case Area::DoorNum::DOWN:
		nowRoom += moveDownRoom;
		break;
	case Area::DoorNum::LEFT:
		nowRoom += moveLeftRoom;
		break;
	case Area::DoorNum::RIGHT:
		nowRoom += moveRightRoom;
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

int Stage::DeleteRoom(int direction)
{
	if (direction < 0 && direction >= 4)
	{
		return FUNCTION_ERROR;
	}

	Vector3 moveRoom = {};

	switch (direction)
	{
	case Area::DoorNum::UP:
		moveRoom = nowRoom + moveUpRoom;
		break;
	case Area::DoorNum::DOWN:
		moveRoom = nowRoom + moveDownRoom;
		break;
	case Area::DoorNum::LEFT:
		moveRoom = nowRoom + moveLeftRoom;
		break;
	case Area::DoorNum::RIGHT:
		moveRoom = nowRoom + moveRightRoom;
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
		case Area::DoorNum::UP:
			moveRoom = nowRoom + moveUpRoom;
			break;
		case Area::DoorNum::DOWN:
			moveRoom = nowRoom + moveDownRoom;
			break;
		case Area::DoorNum::LEFT:
			moveRoom = nowRoom + moveLeftRoom;
			break;
		case Area::DoorNum::RIGHT:
			moveRoom = nowRoom + moveRightRoom;
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
	scroll.ScrollStart();
	return 0;
}

int Stage::MoveUpRoom()
{
	return MoveRoom(nowRoom + moveUpRoom);
}

int Stage::MoveDownRoom()
{
	return MoveRoom(nowRoom + moveDownRoom);
}

int Stage::MoveLeftRoom()
{
	return MoveRoom(nowRoom + moveLeftRoom);
}

int Stage::MoveRightRoom()
{
	return MoveRoom(nowRoom + moveRightRoom);
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
						  (roomPos == nowRoom) ? DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, color.w) : color);
		if (rooms.find(roomPos + moveLeftRoom) != rooms.end() &&
			rooms[roomPos].GetDoorStatus(Area::LEFT) == Door::DoorStatus::OPEN &&
			(rooms[roomPos + moveLeftRoom].isAlive == true))
		{
			if (rooms[roomPos + moveLeftRoom].GetDoorStatus(Area::RIGHT) == Door::DoorStatus::OPEN)
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
		if (rooms.find(roomPos + moveUpRoom) != rooms.end() &&
			rooms[roomPos].GetDoorStatus(Area::UP) == Door::DoorStatus::OPEN &&
			(rooms[roomPos + moveUpRoom].isAlive == true))
		{
			if (rooms[roomPos + moveUpRoom].GetDoorStatus(Area::DOWN) == Door::DoorStatus::OPEN)
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
		if (rooms.find(roomPos + moveRightRoom) != rooms.end() &&
			rooms.at(roomPos).GetDoorStatus(Area::RIGHT) == Door::DoorStatus::OPEN &&
			(rooms[roomPos + moveRightRoom].isAlive == true))
		{
			if (rooms[roomPos + moveRightRoom].GetDoorStatus(Area::LEFT) == Door::DoorStatus::OPEN)
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
		if (rooms.find(roomPos + moveDownRoom) != rooms.end() &&
			rooms.at(roomPos).GetDoorStatus(Area::DOWN) == Door::DoorStatus::OPEN &&
			(rooms[roomPos + moveDownRoom].isAlive == true))
		{

			if (rooms[roomPos + moveDownRoom].GetDoorStatus(Area::UP) == Door::DoorStatus::OPEN)
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

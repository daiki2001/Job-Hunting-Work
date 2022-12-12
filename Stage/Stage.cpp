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
}

void Stage::Draw(int offsetX, int offsetY)
{
	rooms[nowRoom].Draw(offsetX, offsetY);
	MiniMap(DirectXInit::GetInstance()->windowWidth, 0, 20.0f);
}

void Stage::Reset()
{
	nowRoom = { 0.0f, 0.0f, 0.0f };

	for (auto& i : rooms)
	{
		i.second.Reset();
	}
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

int Stage::MoveUpRoom()
{
	Vector3 moveRoom = nowRoom + moveUpRoom;

	if (rooms.find(moveRoom) == rooms.end() ||
		rooms[moveRoom].isAlive == false)
	{
		return FUNCTION_ERROR;
	}

	nowRoom = moveRoom;
	return 0;
}

int Stage::MoveDownRoom()
{
	Vector3 moveRoom = nowRoom + moveDownRoom;

	if (rooms.find(moveRoom) == rooms.end() ||
		rooms[moveRoom].isAlive == false)
	{
		return FUNCTION_ERROR;
	}

	nowRoom = moveRoom;
	return 0;
}

int Stage::MoveLeftRoom()
{
	Vector3 moveRoom = nowRoom + moveLeftRoom;

	if (rooms.find(moveRoom) == rooms.end() ||
		rooms[moveRoom].isAlive == false)
	{
		return FUNCTION_ERROR;
	}

	nowRoom = moveRoom;
	return 0;
}

int Stage::MoveRightRoom()
{
	Vector3 moveRoom = nowRoom + moveRightRoom;

	if (rooms.find(moveRoom) == rooms.end() ||
		rooms[moveRoom].isAlive == false)
	{
		return FUNCTION_ERROR;
	}

	nowRoom = moveRoom;
	return 0;
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

void Stage::MiniMap(int offsetX, int offsetY, float scale)
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
		if (x == 0.0f)
		{
			if (rooms.find(roomPos + moveLeftRoom) != rooms.end() &&
				rooms[roomPos].GetDoorStatus(Area::LEFT) == Door::DoorStatus::OPEN &&
				(rooms[roomPos + moveLeftRoom].isAlive == true))
			{
				draw->DrawTextrue(x * (2.0f + 1.0f) * scale - 1.5f * scale + offset.x,
								  y * (2.0f + 1.0f) * scale + offset.y,
								  1.0f * scale,
								  1.0f * scale,
								  0.0f,
								  Parameter::Get("white1x1"),
								  DirectX::XMFLOAT2(0.5f, 0.5f),
								  color);
			}
		}
		if (y == 0.0f)
		{
			if (rooms.find(roomPos + moveUpRoom) != rooms.end() &&
				rooms[roomPos].GetDoorStatus(Area::UP) == Door::DoorStatus::OPEN &&
				(rooms[roomPos + moveUpRoom].isAlive == true))
			{
				draw->DrawTextrue(x * (2.0f + 1.0f) * scale + offset.x,
								  y * (2.0f + 1.0f) * scale - 1.5f * scale + offset.y,
								  1.0f * scale,
								  1.0f * scale,
								  0.0f,
								  Parameter::Get("white1x1"),
								  DirectX::XMFLOAT2(0.5f, 0.5f),
								  color);
			}
		}
		if (rooms.find(roomPos + moveRightRoom) != rooms.end() &&
			rooms.at(roomPos).GetDoorStatus(Area::RIGHT) == Door::DoorStatus::OPEN &&
			(rooms[roomPos + moveRightRoom].isAlive == true))
		{
			draw->DrawTextrue(x * (2.0f + 1.0f) * scale + 1.5f * scale + offset.x,
							  y * (2.0f + 1.0f) * scale + offset.y,
							  1.0f * scale,
							  1.0f * scale,
							  0.0f,
							  Parameter::Get("white1x1"),
							  DirectX::XMFLOAT2(0.5f, 0.5f),
							  color);
		}
		if (rooms.find(roomPos + moveDownRoom) != rooms.end() &&
			rooms.at(roomPos).GetDoorStatus(Area::DOWN) == Door::DoorStatus::OPEN &&
			(rooms[roomPos + moveDownRoom].isAlive == true))
		{
			draw->DrawTextrue(x * (2.0f + 1.0f) * scale + offset.x,
							  y * (2.0f + 1.0f) * scale + 1.5f * scale + offset.y,
							  1.0f * scale,
							  1.0f * scale,
							  0.0f,
							  Parameter::Get("white1x1"),
							  DirectX::XMFLOAT2(0.5f, 0.5f),
							  color);
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
				reslut = true;
				break;
			}
		}
	}

	return reslut;
}

#include "Stage.h"

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
}

void Stage::Reset()
{
	nowRoom = { 0.0f, 0.0f, 0.0f };

	for (auto& i : rooms)
	{
		i.second.Reset();
	}
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
	static const Vector3 moveUpRoom = { 0.0f, -1.0f, 0.0f };
	static const Vector3 moveDownRoom = { 0.0f, 1.0f, 0.0f };
	static const Vector3 moveLeftRoom = { -1.0f, 0.0f, 0.0f };
	static const Vector3 moveRightRoom = { 1.0f, 0.0f, 0.0f };

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
		return 0;
	}

	rooms[nowRoom] = Area::GetPlaneArea();

	return direction;
}

int Stage::MoveUpRoom()
{
	static const Vector3 moveRoom = { 0.0f, -1.0f, 0.0f };

	if (rooms.find(nowRoom + moveRoom) == rooms.end())
	{
		return FUNCTION_ERROR;
	}
	else
	{
		nowRoom += moveRoom;
	}

	return 0;
}

int Stage::MoveDownRoom()
{
	static const Vector3 moveRoom = { 0.0f, 1.0f, 0.0f };

	if (rooms.find(nowRoom + moveRoom) == rooms.end())
	{
		return FUNCTION_ERROR;
	}
	else
	{
		nowRoom += moveRoom;
	}

	return 0;
}

int Stage::MoveLeftRoom()
{
	static const Vector3 moveRoom = { -1.0f, 0.0f, 0.0f };

	if (rooms.find(nowRoom + moveRoom) == rooms.end())
	{
		return FUNCTION_ERROR;
	}
	else
	{
		nowRoom += moveRoom;
	}

	return 0;
}

int Stage::MoveRightRoom()
{
	static const Vector3 moveRoom = { 1.0f, 0.0f, 0.0f };

	if (rooms.find(nowRoom + moveRoom) == rooms.end())
	{
		return FUNCTION_ERROR;
	}
	else
	{
		nowRoom += moveRoom;
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

const bool Stage::IsGoal()
{
	static bool reslut = false;
	reslut = false;

	for (auto& i : rooms)
	{
		if (i.second.IsGoal())
		{
			reslut = true;
			break;
		}
	}

	return reslut;
}

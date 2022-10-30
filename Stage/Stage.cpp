#include "Stage.h"

DrawPolygon* Stage::draw = nullptr;
std::vector<Stage::Room> Stage::room;
int Stage::nowRoom = 0;

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
	nowRoom = 0;
}

void Stage::Update()
{
	room[nowRoom].area.Update();
}

void Stage::Draw(int offsetX, int offsetY)
{
	room[nowRoom].area.Draw(offsetX, offsetY);
}

void Stage::Reset()
{
	nowRoom = 0;

	for (size_t i = 0; i < room.size(); i++)
	{
		room[i].area.Reset();
	}
}

int Stage::LoadStage(const char* filePath)
{
	if (filePath == nullptr)
	{
		OutputDebugStringA("ファイル名が間違っています。\n");
		return FUNCTION_ERROR;
	}

	room.clear();

	FILE* fileHandle;
	errno_t err;
	char string[256] = { 0 };

	size_t roomNum = 0;

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

	for (size_t i = 0; i < roomNum; i++)
	{
		room.emplace_back();
		File::LoadMapChip(fileHandle, room[i].connection, 4, -2);
		room.back().area.LoadArea(fileHandle);
	}

	fclose(fileHandle);
	return 0;
}

int Stage::WirteStage(const char* filePath)
{
	FILE* fileHandle;
	File::WriteFileOpen(&fileHandle, filePath);

	// 部屋数
	fprintf_s(fileHandle, "%zu\n", room.size());
	// 各部屋の情報
	for (size_t i = 0; i < room.size(); i++)
	{
		// 部屋との繋がり
		File::WriteCSV(fileHandle, room[i].connection, 4);

		room[i].area.WriteArea(fileHandle);
	}

	fclose(fileHandle);
	return 0;
}

int Stage::CreateRoom(int direction)
{
	if (direction >= 0 && direction < 4)
	{
		if (room[nowRoom].connection[direction] != -1)
		{
			return FUNCTION_ERROR;
		}
	}
	else
	{
		if (room.empty() == false)
		{
			return FUNCTION_ERROR;
		}
	}

	room.emplace_back();

	for (int i = 0; i < Area::STAGE_WIDTH * Area::STAGE_HEIGHT; i++)
	{
		room.back().area.GetBlockManager()->CreateBlock(BlockManager::TypeId::NONE);
		room.back().area.GetBlockManager()->GetBlock(i).pos.x = static_cast<float>(i % Area::STAGE_WIDTH) * 1.0f;
		room.back().area.GetBlockManager()->GetBlock(i).pos.y = static_cast<float>(i / Area::STAGE_WIDTH) * -1.0f;
	}

	for (int i = 0; i < 4; i++)
	{
		room.back().area.SetDoorStatus(Door::DoorStatus::OPEN, Area::DoorNum(i));
	}

	switch (direction)
	{
	case Area::DoorNum::UP:
		room[nowRoom].connection[Area::DoorNum::UP] = static_cast<int>(room.size() - 1);
		room.back().connection[Area::DoorNum::DOWN] = nowRoom;
		break;
	case Area::DoorNum::DOWN:
		room[nowRoom].connection[Area::DoorNum::DOWN] = static_cast<int>(room.size() - 1);
		room.back().connection[Area::DoorNum::UP] = nowRoom;
		break;
	case Area::DoorNum::LEFT:
		room[nowRoom].connection[Area::DoorNum::LEFT] = static_cast<int>(room.size() - 1);
		room.back().connection[Area::DoorNum::RIGHT] = nowRoom;
		break;
	case Area::DoorNum::RIGHT:
		room[nowRoom].connection[Area::DoorNum::RIGHT] = static_cast<int>(room.size() - 1);
		room.back().connection[Area::DoorNum::LEFT] = nowRoom;
		break;
	default:
		break;
	}

	nowRoom = static_cast<int>(room.size() - 1);
	room.back().area.SetDoorInit(GetDoorStatus(Area::DoorNum::UP),
								 GetDoorStatus(Area::DoorNum::DOWN),
								 GetDoorStatus(Area::DoorNum::LEFT),
								 GetDoorStatus(Area::DoorNum::RIGHT));

	return direction;
}

int Stage::MoveUpRoom()
{
	if (room[nowRoom].connection[Area::DoorNum::UP] == -1)
	{
		return FUNCTION_ERROR;
	}

	nowRoom = room[nowRoom].connection[Area::DoorNum::UP];

	return 0;
}

int Stage::MoveDownRoom()
{
	if (room[nowRoom].connection[Area::DoorNum::DOWN] == -1)
	{
		return FUNCTION_ERROR;
	}

	nowRoom = room[nowRoom].connection[Area::DoorNum::DOWN];

	return 0;
}

int Stage::MoveLeftRoom()
{
	if (room[nowRoom].connection[Area::DoorNum::LEFT] == -1)
	{
		return FUNCTION_ERROR;
	}

	nowRoom = room[nowRoom].connection[Area::DoorNum::LEFT];

	return 0;
}

int Stage::MoveRightRoom()
{
	if (room[nowRoom].connection[Area::DoorNum::RIGHT] == -1)
	{
		return FUNCTION_ERROR;
	}

	nowRoom = room[nowRoom].connection[Area::DoorNum::RIGHT];

	return 0;
}

void Stage::AllDeleteRoom()
{
	for (size_t i = 0; i < room.size(); i++)
	{
		room[i].area.GetBlockManager()->AllDeleteBlock();
	}
	room.clear();
}

const bool Stage::IsGoal()
{
	static bool reslut = false;
	reslut = false;

	for (size_t i = 0; i < room.size(); i++)
	{
		if (room[i].area.IsGoal())
		{
			reslut = true;
			break;
		}
	}

	return reslut;
}

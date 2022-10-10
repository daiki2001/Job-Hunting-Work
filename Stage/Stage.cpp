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
			roomNum += string[i] - '0';
		}
	}

	for (size_t i = 0; i < roomNum; i++)
	{
		room.emplace_back();
		Load::LoadMapChip(fileHandle, room[i].connection, 4, -2);
		room.back().area.LoadArea(fileHandle);
	}

	fclose(fileHandle);
	return 0;
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

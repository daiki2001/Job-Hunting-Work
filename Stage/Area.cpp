#include "Area.h"
#include "Door.h"
#include "./Header/DirectXInit.h"
#include "./Input/GameInput.h"
#include <math.h>

namespace
{
static GameInput* input = GameInput::Get();
}

const float Area::WALL_SIZE = 2.0f;
const Math::Vector3 Area::INIT_CAMERA = { 7.0f, -3.0f, 0.0f };
const int Area::NONE_LOST_FOREST = Area::DoorNum::MAX;
const size_t Area::MAX_COURSE_NUM = 5;
DrawPolygon* Area::draw = nullptr;
int Area::wall_obj = FUNCTION_ERROR;
Area* Area::planeArea = nullptr;

Area::Area() :
	block_mgr{},
	door{},
	doorInit{},
	lostForest{},
	isAlive(false)
{
	block_mgr.Init(Area::draw);

	door[DoorNum::LEFT].Init(Vector3(-9.0f, 0.0f, 0.0f), Vector3(1.0f, Door::DOOR_WIDTH, WALL_SIZE), Door::DoorStatus::CLOSE);
	door[DoorNum::RIGHT].Init(Vector3(+9.0f, 0.0f, 0.0f), Vector3(1.0f, Door::DOOR_WIDTH, WALL_SIZE), Door::DoorStatus::CLOSE);
	door[DoorNum::TOP].Init(Vector3(0.0f, +5.0f, 0.0f), Vector3(Door::DOOR_WIDTH, 1.0f, WALL_SIZE), Door::DoorStatus::CLOSE);
	door[DoorNum::BOTTOM].Init(Vector3(0.0f, -5.0f, 0.0f), Vector3(Door::DOOR_WIDTH, 1.0f, WALL_SIZE), Door::DoorStatus::CLOSE);

	SetDoorInit(door);
}

Area::~Area()
{
}

void Area::StaticInit(DrawPolygon* const draw)
{
	if (draw != nullptr)
	{
		Area::draw = draw;
	}
	// 外壁のモデルの読み込み
	wall_obj = Area::draw->CreateOBJModel("./Resources/Game/Wall/Wall.obj", "./Resources/Game/Wall/");

	// planeAreaの初期化
	static Area defArea = {};
	static bool isCreatePlane = false;

	if (isCreatePlane == false)
	{
		planeArea = &defArea;
		isCreatePlane = true;

		for (int i = 0; i < BlockManager::STAGE_WIDTH * BlockManager::STAGE_HEIGHT; i++)
		{
			planeArea->GetBlockManager()->CreateBlock(BlockManager::TypeId::NONE);
			planeArea->GetBlockManager()->GetBlock(i).pos.x = static_cast<float>(i % BlockManager::STAGE_WIDTH) * 1.0f;
			planeArea->GetBlockManager()->GetBlock(i).pos.y = static_cast<float>(i / BlockManager::STAGE_WIDTH) * -1.0f;
			planeArea->GetBlockManager()->GetBlock(i).initPos = planeArea->GetBlockManager()->GetBlock(i).pos;
		}
		planeArea->SetDoorInit(Door::DoorStatus::OPEN, Door::DoorStatus::OPEN,
							   Door::DoorStatus::OPEN, Door::DoorStatus::OPEN);
		planeArea->Reset();
		planeArea->lostForest.reserve(MAX_COURSE_NUM + 1U);
		for (size_t i = 0; i < planeArea->lostForest.capacity(); i++)
		{
			planeArea->lostForest.emplace_back(NONE_LOST_FOREST);
		}
	}

	Door::StaticInit(draw);
}

void Area::Reset()
{
	block_mgr.Reset();

	for (size_t i = 0; i < DOOR_COUNT; i++)
	{
		door[i] = doorInit[i];
	}
}

void Area::Update()
{
	for (auto& i : door)
	{
		i.EaseUpdate();
	}

	if (isAlive == false) return;

	block_mgr.Update();

	if (block_mgr.GetDoor())
	{
		for (auto& i : door)
		{
			i.Open();
		}
	}
}

void Area::Draw(const Vector3& offset)
{
	if (isAlive == false) return;

	static const float halfWidth = floorf(Door::DOOR_WIDTH / 2.0f);
	const Vector3 centerPos = INIT_CAMERA + offset;

	// 外壁の描画
	DirectDrawing::ChangeMaterialShader();
	DrawWall(centerPos);

	door[DoorNum::LEFT].Draw(centerPos);
	for (float i = 0; i < Door::DOOR_WIDTH; i += 1.0f)
	{
		BlockType::FloorDraw(Vector3(-8.0f,i - halfWidth,  0.0f) + centerPos);
	}
	door[DoorNum::RIGHT].Draw(centerPos);
	for (float i = 0; i < Door::DOOR_WIDTH; i += 1.0f)
	{
		BlockType::FloorDraw(Vector3(+8.0f, i - halfWidth, 0.0f) + centerPos);
	}
	door[DoorNum::TOP].Draw(centerPos);
	for (float i = 0; i < Door::DOOR_WIDTH; i += 1.0f)
	{
		BlockType::FloorDraw(Vector3(i - halfWidth, +4.0f, 0.0f) + centerPos);
	}
	door[DoorNum::BOTTOM].Draw(centerPos);
	for (float i = 0; i < Door::DOOR_WIDTH; i += 1.0f)
	{
		BlockType::FloorDraw(Vector3(i - halfWidth, -4.0f, 0.0f) + centerPos);
	}

	block_mgr.Draw(offset);
}

int Area::LoadArea(FILE* fileHandle)
{
	int mapArray[BlockManager::STAGE_WIDTH * BlockManager::STAGE_HEIGHT] = { BlockManager::TypeId::NONE };
	int doorSetting[DOOR_COUNT] = { Door::DoorStatus::OPEN };
	int courceSetting[MAX_COURSE_NUM] = {};

	File::LoadMapChip(fileHandle, doorSetting, static_cast<int>(DOOR_COUNT));
	File::LoadMapChip(fileHandle, courceSetting, MAX_COURSE_NUM, true, NONE_LOST_FOREST);
	File::LoadMapChip(fileHandle, mapArray, sizeof(mapArray) / sizeof(mapArray[0]));

	block_mgr.AllDeleteBlock();

	// マップの設定
	for (int i = 0; i < sizeof(mapArray) / sizeof(mapArray[0]); i++)
	{
		int x = i % BlockManager::STAGE_WIDTH;
		int y = i / BlockManager::STAGE_WIDTH;
		int index = block_mgr.CreateBlock(BlockManager::TypeId(mapArray[i]));

		if (index == Engine::FUNCTION_ERROR)
		{
			continue;
		}

		auto& block = block_mgr.GetBlock(index);
		block.pos.x = static_cast<float>(x * 1.0f);
		block.pos.y = static_cast<float>(-y * 1.0f);
		block.initPos = block.pos;
		block.initPos.z = 0.0f;
	}

	// ドアの設定
	for (int i = 0; i < static_cast<int>(DOOR_COUNT); i++)
	{
		static Vector3 size = Vector3(3.0f, 1.0f, 2.0f);

		if (doorSetting[i] == static_cast<int>(Door::DoorStatus::ENTRANCE))
		{
			doorSetting[i] = static_cast<int>(Door::DoorStatus::WALL);
		}

		if ((i % 4) == DoorNum::LEFT || (i % 4) == DoorNum::RIGHT)
		{
			size.x = 1.0f;
			size.y = 3.0f;
		}
		else if ((i % 4) == DoorNum::TOP || (i % 4) == DoorNum::BOTTOM)
		{
			size.x = 3.0f;
			size.y = 1.0f;
		}

		switch (i)
		{
		case DoorNum::LEFT:
			door[i].Init(Vector3(-9.0f, 0.0f, 0.0f), size, static_cast<Door::DoorStatus>(doorSetting[i]));
			break;
		case DoorNum::RIGHT:
			door[i].Init(Vector3(+9.0f, 0.0f, 0.0f), size, static_cast<Door::DoorStatus>(doorSetting[i]));
			break;
		case DoorNum::TOP:
			door[i].Init(Vector3(0.0f, +5.0f, 0.0f), size, static_cast<Door::DoorStatus>(doorSetting[i]));
			break;
		case DoorNum::BOTTOM:
			door[i].Init(Vector3(0.0f, -5.0f, 0.0f), size, static_cast<Door::DoorStatus>(doorSetting[i]));
			break;
		default:
			break;
		}
		doorInit[i] = door[i];
	}

	// 迷いの森の道設定
	for (size_t i = 0; i < MAX_COURSE_NUM; i++)
	{
		if (lostForest.size() < MAX_COURSE_NUM + 1U)
		{
			lostForest.emplace_back(courceSetting[i]);
		}
		else
		{
			lostForest[i] = courceSetting[i];
		}

		if (courceSetting[i] == NONE_LOST_FOREST)
		{
			break;
		}
	}
	lostForest.emplace_back(NONE_LOST_FOREST);

	isAlive = true;
	return 0;
}

int Area::WriteArea(FILE* fileHandle)
{
	if (isAlive == false) return FUNCTION_ERROR;

	// ドア・壁
	int doorStatus[DOOR_COUNT] = {};
	for (size_t i = 0; i < DOOR_COUNT; i++)
	{
		doorStatus[i] = static_cast<int>(door[i].GetStatus());
	}
	File::WriteCSV(fileHandle, doorStatus, DOOR_COUNT);

	// 迷いの森の道
	int courceSetting[MAX_COURSE_NUM] = {};
	size_t courceSettingSize = 0;
	for (courceSettingSize = 0; courceSettingSize < lostForest.size();)
	{
		courceSetting[courceSettingSize] = lostForest[courceSettingSize];

		if (lostForest[courceSettingSize] == NONE_LOST_FOREST)
		{
			break;
		}

		courceSettingSize++;
	}
	File::WriteCSV(fileHandle, courceSetting, courceSettingSize + 1U);

	// ブロック情報
	for (size_t i = 0; i < BlockManager::STAGE_HEIGHT; i++)
	{
		int blocks[BlockManager::STAGE_WIDTH] = {};
		for (size_t j = 0; j < BlockManager::STAGE_WIDTH; j++)
		{
			blocks[j] = static_cast<int>(block_mgr.GetBlock(i * BlockManager::STAGE_WIDTH + j).typeId);
		}
		File::WriteCSV(fileHandle, blocks, BlockManager::STAGE_WIDTH);
	}
	return 0;
}

void Area::DrawWall(const Vector3& offset)
{
	const Vector3 size = Vector3::Scale_xyz(WALL_SIZE);

	draw->DrawOBJ(wall_obj, Vector3(-8.5f, +4.5f, 0.0f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(-8.5f, -4.5f, 0.0f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+8.5f, -4.5f, 0.0f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+8.5f, +4.5f, 0.0f) + offset, Math::Identity(), size);

	draw->DrawOBJ(wall_obj, Vector3(-8.5f, +2.5f, 0.0f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(-8.5f, -2.5f, 0.0f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+8.5f, -2.5f, 0.0f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+8.5f, +2.5f, 0.0f) + offset, Math::Identity(), size);

	draw->DrawOBJ(wall_obj, Vector3(-6.5f, +4.5f, 0.0f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(-6.5f, -4.5f, 0.0f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+6.5f, -4.5f, 0.0f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+6.5f, +4.5f, 0.0f) + offset, Math::Identity(), size);

	draw->DrawOBJ(wall_obj, Vector3(-4.5f, +4.5f, 0.0f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(-4.5f, -4.5f, 0.0f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+4.5f, -4.5f, 0.0f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+4.5f, +4.5f, 0.0f) + offset, Math::Identity(), size);

	draw->DrawOBJ(wall_obj, Vector3(-2.5f, +4.5f, 0.0f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(-2.5f, -4.5f, 0.0f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+2.5f, -4.5f, 0.0f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+2.5f, +4.5f, 0.0f) + offset, Math::Identity(), size);
}

const int Area::LostForest(const std::vector<int>& route, const size_t& index)
{
	if (index >= lostForest.size() ||
		lostForest[index] == NONE_LOST_FOREST)
	{
		return 0;
	}

	if (index >= route.size())
	{
		return 1;
	}

	if (route[index] < 0 || route[index] > NONE_LOST_FOREST)
	{
		return FUNCTION_ERROR;
	}
	else if (route[index] >= 0 && route[index] < NONE_LOST_FOREST)
	{
		if (route[index] == lostForest[index])
		{
			return LostForest(route, index + 1);
		}
		else
		{
			bool isLoop = false;

			for (size_t i = 0; i < lostForest.size(); i++)
			{
				if (route[index] == lostForest[i])
				{
					isLoop = true;
					break;
				}
			}

			if (isLoop)
			{
				return 2;
			}
			else
			{
				return 0;
			}
		}
	}

	return 0;
}

void Area::SetDoorInit(Door doors[DOOR_COUNT])
{
	for (size_t i = 0; i < DOOR_COUNT; i++)
	{
		doorInit[i] = doors[i];
	}
}

void Area::SetDoorInit(Door::DoorStatus up, Door::DoorStatus down,
					   Door::DoorStatus left, Door::DoorStatus right)
{
	doorInit[DoorNum::TOP].SetStatus(up);
	doorInit[DoorNum::BOTTOM].SetStatus(down);
	doorInit[DoorNum::LEFT].SetStatus(left);
	doorInit[DoorNum::RIGHT].SetStatus(right);
}

int Area::SetRoute(const size_t& index, int route)
{
	if (index > MAX_COURSE_NUM) return FUNCTION_ERROR;

	if (index < lostForest.size())
	{
		lostForest[index] = route;
	}
	else
	{
		lostForest.emplace_back(route);
	}

	return 0;
}

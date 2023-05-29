#include "Area.h"
#include "Door.h"
#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Input/GameInput.h"
#include "./Header/EngineGeneral.h"
#include <math.h>

namespace
{
static GameInput* input = GameInput::Get();
}

const float Area::WALL_SIZE = 2.0f;
const Math::Vector3 Area::INIT_CAMERA = { 7.0f, -3.0f, 0.0f };
const int Area::NONE_LOST_FOREST = Player::Direction::COUNT;
const size_t Area::MAX_COURSE_NUM = 5;
int Area::wall_obj = FUNCTION_ERROR;
int Area::floorObj = FUNCTION_ERROR;
Area* Area::planeArea = nullptr;

Area::Area() :
	block_mgr{},
	door{},
	doorInit{},
	lostForest{},
	isAlive(false)
{
	float sidePos = 9.0f;
	float topPos = 5.0f;
	block_mgr.Init();

	door[DoorNum::LEFT].Init(Vector3(-(sidePos - 0.5f), 0.0f, 0.0f), Vector3(WALL_SIZE, Door::DOOR_WIDTH, WALL_SIZE), Door::DoorStatus::CLOSE);
	door[DoorNum::LEFT_TOP].Init(Vector3(-sidePos, +2.5f, 0.0f), Vector3(1.0f, WALL_SIZE, WALL_SIZE), Door::DoorStatus::WALL);
	door[DoorNum::LEFT_BOTTOM].Init(Vector3(-sidePos, -2.5f, 0.0f), Vector3(1.0f, WALL_SIZE, WALL_SIZE), Door::DoorStatus::WALL);

	door[DoorNum::RIGHT].Init(Vector3(+(sidePos - 0.5f), 0.0f, 0.0f), Vector3(WALL_SIZE, Door::DOOR_WIDTH, WALL_SIZE), Door::DoorStatus::CLOSE);
	door[DoorNum::RIGHT_TOP].Init(Vector3(+sidePos, +2.5f, 0.0f), Vector3(1.0f, WALL_SIZE, WALL_SIZE), Door::DoorStatus::WALL);
	door[DoorNum::RIGHT_BOTTOM].Init(Vector3(+sidePos, -2.5f, 0.0f), Vector3(1.0f, WALL_SIZE, WALL_SIZE), Door::DoorStatus::WALL);

	door[DoorNum::TOP].Init(Vector3(0.0f, +topPos, 0.0f), Vector3(Door::DOOR_WIDTH, 1.0f, WALL_SIZE), Door::DoorStatus::CLOSE);
	door[DoorNum::TOP_LEFT].Init(Vector3(-6.5f, +topPos, 0.0f), Vector3(WALL_SIZE, 1.0f, WALL_SIZE), Door::DoorStatus::WALL);
	door[DoorNum::TOP_RIGHT].Init(Vector3(+6.5f, +topPos, 0.0f), Vector3(WALL_SIZE, 1.0f, WALL_SIZE), Door::DoorStatus::WALL);

	door[DoorNum::BOTTOM].Init(Vector3(0.0f, -topPos, 0.0f), Vector3(Door::DOOR_WIDTH, 1.0f, WALL_SIZE), Door::DoorStatus::CLOSE);
	door[DoorNum::BOTTOM_LEFT].Init(Vector3(-6.5f, -topPos, 0.0f), Vector3(WALL_SIZE, 1.0f, WALL_SIZE), Door::DoorStatus::WALL);
	door[DoorNum::BOTTOM_RIGHT].Init(Vector3(+6.5f, -topPos, 0.0f), Vector3(WALL_SIZE, 1.0f, WALL_SIZE), Door::DoorStatus::WALL);

	SetDoorInit(door);
}

Area::~Area()
{
}

void Area::StaticInit()
{
	auto draw = Library::DrawPolygon::GetInstance();

	// 外壁のモデルの読み込み
	wall_obj = draw->CreateOBJModel("./Resources/Game/Wall/Wall.obj", "./Resources/Game/Wall/");
	// 床のオブジェクトの生成
	floorObj = draw->Create3Dbox(Vector3(Door::DOOR_WIDTH * 5.0f, 3.0f, 1.0f));
	draw->Tiring(floorObj, Door::DOOR_WIDTH * 5.0f, 3.0f);

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

	Door::StaticInit();
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

	static const float halfWidth = floorf(Door::DOOR_WIDTH * 5.0f / 2.0f);
	const Vector3 centerPos = INIT_CAMERA + offset;

	// 外壁の描画
	DrawWall(centerPos);

	for (int i = 0; i < DOOR_COUNT; i++)
	{
		door[i].Draw(centerPos);
	}
	for (int i = 0; i < Player::Direction::COUNT; i++)
	{
		Vector3 pos = Vector3(0.0f, 0.0f, 1.0f);
		Math::Matrix4 rota = Math::Identity();

		if (i - 1 == Player::Direction::LEFT)
		{
			pos.x = -9.0f;
			rota = Math::rotateZ(Math::PI_F / 2.0f);
		}
		else if (i - 1 == Player::Direction::RIGHT)
		{
			pos.x = +9.0f;
			rota = Math::rotateZ(Math::PI_F / 2.0f);
		}
		else if (i - 1 == Player::Direction::TOP)
		{
			pos.y = +5.0f;
		}
		else if (i - 1 == Player::Direction::BOTTOM)
		{
			pos.y = -5.0f;
		}
		pos += centerPos;

		DirectDrawing::ChangeOBJShader();
		Library::DrawPolygon::GetInstance()->Draw(floorObj,
												  pos,
												  rota,
												  Vector3::Scale_xyz(1.0f),
												  Color::AddAlphaValue(Color::WHITE),
												  BlockType::GetFloorGraph());
	}

	block_mgr.Draw(offset);
}

int Area::LoadArea(FILE* fileHandle)
{
	int mapArray[BlockManager::STAGE_WIDTH * BlockManager::STAGE_HEIGHT] = { BlockManager::TypeId::NONE };
	int doorSetting[DOOR_COUNT] = { Door::DoorStatus::WALL };
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
		if (doorSetting[i] == static_cast<int>(Door::DoorStatus::ENTRANCE))
		{
			doorSetting[i] = static_cast<int>(Door::DoorStatus::WALL);
		}

		door[i].Init(door[i].GetPos(), door[i].GetSize(), static_cast<Door::DoorStatus>(doorSetting[i]));
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
	auto draw = Library::DrawPolygon::GetInstance();

	DirectDrawing::ChangeMaterialShader();
	draw->DrawOBJ(wall_obj, Vector3(-8.5f, +4.5f, -0.5f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(-8.5f, -4.5f, -0.5f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+8.5f, -4.5f, -0.5f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+8.5f, +4.5f, -0.5f) + offset, Math::Identity(), size);

	draw->DrawOBJ(wall_obj, Vector3(-4.5f, +4.5f, -0.5f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(-4.5f, -4.5f, -0.5f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+4.5f, -4.5f, -0.5f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+4.5f, +4.5f, -0.5f) + offset, Math::Identity(), size);

	draw->DrawOBJ(wall_obj, Vector3(-2.5f, +4.5f, -0.5f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(-2.5f, -4.5f, -0.5f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+2.5f, -4.5f, -0.5f) + offset, Math::Identity(), size);
	draw->DrawOBJ(wall_obj, Vector3(+2.5f, +4.5f, -0.5f) + offset, Math::Identity(), size);
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

#include "Area.h"
#include "Door.h"
#include "InputManager.h"

namespace
{
static InputManager* input = InputManager::Get();
}

const int Area::STAGE_WIDTH = 15;
const int Area::STAGE_HEIGHT = 7;
DrawPolygon* Area::draw = nullptr;
int Area::wall_obj = FUNCTION_ERROR;
Area* Area::planeArea = nullptr;

Area::Area() :
	block_mgr{},
	door{},
	doorInit{},
	isAlive(false)
{
	block_mgr.Init(Area::draw);

	door[DoorNum::UP].Init(Vector3(3.0f, 1.0f, 2.0f), Door::DoorStatus::CLOSE);
	door[DoorNum::DOWN].Init(Vector3(3.0f, 1.0f, 2.0f), Door::DoorStatus::CLOSE);
	door[DoorNum::LEFT].Init(Vector3(1.0f, 3.0f, 2.0f), Door::DoorStatus::CLOSE);
	door[DoorNum::RIGHT].Init(Vector3(1.0f, 3.0f, 2.0f), Door::DoorStatus::CLOSE);

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
	planeArea = &defArea;
	for (int i = 0; i < Area::STAGE_WIDTH * Area::STAGE_HEIGHT; i++)
	{
		planeArea->GetBlockManager()->CreateBlock(BlockManager::TypeId::NONE);
		planeArea->GetBlockManager()->GetBlock(i).pos.x = static_cast<float>(i % Area::STAGE_WIDTH) * 1.0f;
		planeArea->GetBlockManager()->GetBlock(i).pos.y = static_cast<float>(i / Area::STAGE_WIDTH) * -1.0f;
	}
	planeArea->SetDoorInit(Door::DoorStatus::OPEN, Door::DoorStatus::OPEN,
						   Door::DoorStatus::OPEN, Door::DoorStatus::OPEN);
	planeArea->Reset();

	Door::StaticInit(draw);
}

void Area::Reset()
{
	block_mgr.Reset();

	for (size_t i = 0; i < 4; i++)
	{
		door[i] = doorInit[i];
	}
}

void Area::Update()
{
	if (isAlive == false) return;

	block_mgr.Update();

	if (block_mgr.GetDoor())
	{
		for (size_t i = 0; i < sizeof(door) / sizeof(door[0]); i++)
		{
			door[i].Open();
		}
	}
}

void Area::Draw(int offsetX, int offsetY)
{
	if (isAlive == false) return;

	const Vector3 offset = Vector3(7.0f, -3.0f, 0.0f) +
		Vector3(static_cast<float>(offsetX), static_cast<float>(offsetY), 0.0f);

	// 外壁の描画
	DirectDrawing::ChangeMaterialShader();
	DrawWall(offset);

	door[DoorNum::UP].Draw(Vector3(0.0f, +4.5f, 0.0f) + offset);
	door[DoorNum::DOWN].Draw(Vector3(0.0f, -4.5f, 0.0f) + offset);
	door[DoorNum::LEFT].Draw(Vector3(-8.5f, 0.0f, 0.0f) + offset);
	door[DoorNum::RIGHT].Draw(Vector3(+8.5f, 0.0f, 0.0f) + offset);

	block_mgr.Draw(offsetX, -offsetY);
}

int Area::LoadArea(FILE* fileHandle)
{
	int mapArray[STAGE_WIDTH * STAGE_HEIGHT] = { BlockManager::TypeId::NONE };
	int doorSetting[4] = { Door::DoorStatus::OPEN };

	File::LoadMapChip(fileHandle, doorSetting, 4);
	File::LoadMapChip(fileHandle, mapArray, sizeof(mapArray) / sizeof(mapArray[0]));

	block_mgr.AllDeleteBlock();

	/*マップの設定*/
	for (int i = 0; i < sizeof(mapArray) / sizeof(mapArray[0]); i++)
	{
		int x = i % STAGE_WIDTH;
		int y = i / STAGE_WIDTH;
		int index = block_mgr.CreateBlock(BlockManager::TypeId(mapArray[i]));

		if (index == Engine::FUNCTION_ERROR)
		{
			continue;
		}

		auto& block = block_mgr.GetBlock(index);
		block.pos.x = static_cast<float>(x * 1.0f);
		block.pos.y = static_cast<float>(-y * 1.0f);
	}
	/*ドアの設定*/
	for (int i = 0; i < 4; i++)
	{
		static Vector3 size = Vector3(3.0f, 1.0f, 2.0f);

		if (doorSetting[i] == static_cast<int>(Door::DoorStatus::ENTRANCE))
		{
			doorSetting[i] = static_cast<int>(Door::DoorStatus::OPEN);
		}

		if (i % 4 <= 1)
		{
			size.x = 3.0f;
			size.y = 1.0f;
		}
		else if (i % 4 <= 3)
		{
			size.x = 1.0f;
			size.y = 3.0f;
		}

		door[i].Init(size, static_cast<Door::DoorStatus>(doorSetting[i]));
		doorInit[i] = door[i];
	}

	isAlive = true;
	return 0;
}

int Area::WriteArea(FILE* fileHandle)
{
	if (isAlive == false) return FUNCTION_ERROR;

	// ドア・壁
	int doorStatus[4] = {};
	for (size_t i = 0; i < 4; i++)
	{
		doorStatus[i] = static_cast<int>(door[i].GetStatus());
	}
	File::WriteCSV(fileHandle, doorStatus, 4);

	// ブロック情報
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		int blocks[STAGE_WIDTH] = {};
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			blocks[j] = static_cast<int>(block_mgr.GetBlock(i * STAGE_WIDTH + j).typeId);
		}
		File::WriteCSV(fileHandle, blocks, STAGE_WIDTH);
	}
	return 0;
}

void Area::DrawWall(const Vector3& offset)
{
	draw->DrawOBJ(wall_obj, Vector3(-8.5f, +4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(-8.5f, -4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+8.5f, -4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+8.5f, +4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));

	draw->DrawOBJ(wall_obj, Vector3(-8.5f, +2.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(-8.5f, -2.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+8.5f, -2.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+8.5f, +2.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));

	draw->DrawOBJ(wall_obj, Vector3(-6.5f, +4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(-6.5f, -4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+6.5f, -4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+6.5f, +4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));

	draw->DrawOBJ(wall_obj, Vector3(-4.5f, +4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(-4.5f, -4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+4.5f, -4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+4.5f, +4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));

	draw->DrawOBJ(wall_obj, Vector3(-2.5f, +4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(-2.5f, -4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+2.5f, -4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+2.5f, +4.5f, 0.0f) + offset, Math::Identity(), Vector3(2.0f, 2.0f, 2.0f));
}

void Area::SetDoorInit(Door doors[4])
{
	for (size_t i = 0; i < 4; i++)
	{
		doorInit[i] = doors[i];
	}
}

void Area::SetDoorInit(Door::DoorStatus up, Door::DoorStatus down,
					   Door::DoorStatus left, Door::DoorStatus right)
{
	doorInit[DoorNum::UP].SetStatus(up);
	doorInit[DoorNum::DOWN].SetStatus(down);
	doorInit[DoorNum::LEFT].SetStatus(left);
	doorInit[DoorNum::RIGHT].SetStatus(right);
}

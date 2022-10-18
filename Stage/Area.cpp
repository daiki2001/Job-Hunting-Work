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

Area::Area() :
	block_mgr{},
	door{},
	doorInit{}
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
	block_mgr.Update();

	if (block_mgr.GetDoor())
	{
		for (size_t i = 0; i < sizeof(door) / sizeof(door[0]); i++)
		{
			door[i].Open();
		}
	}

	if (input->DecisionTrigger() && Player::Get()->GetKeyCount() > 0)
	{
		int playerPos = block_mgr.GetSurroundingBlock(0, nullptr);

		if (playerPos % 15 == 0 && playerPos / 15 >= 2 && playerPos / 15 <= 4 &&
			door[DoorNum::LEFT].GetStatus() == Door::DoorStatus::KEY_CLOSE)
		{
			door[DoorNum::LEFT].KeyOpen();
			Player::Get()->UseKey();
		}
		if (playerPos % 15 == 14 && playerPos / 15 >= 2 && playerPos / 15 <= 4 &&
			door[DoorNum::RIGHT].GetStatus() == Door::DoorStatus::KEY_CLOSE)
		{
			door[DoorNum::RIGHT].KeyOpen();
			Player::Get()->UseKey();
		}
		if (playerPos >= 6 && playerPos <= 8 &&
			door[DoorNum::UP].GetStatus() == Door::DoorStatus::KEY_CLOSE)
		{
			door[DoorNum::UP].KeyOpen();
			Player::Get()->UseKey();
		}
		if (playerPos >= 96 && playerPos <= 98 &&
			door[DoorNum::DOWN].GetStatus() == Door::DoorStatus::KEY_CLOSE)
		{
			door[DoorNum::DOWN].KeyOpen();
			Player::Get()->UseKey();
		}
	}
}

void Area::Draw(int offsetX, int offsetY)
{
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

	Load::LoadMapChip(fileHandle, doorSetting, 4, -2);
	Load::LoadMapChip(fileHandle, mapArray, sizeof(mapArray) / sizeof(mapArray[0]));

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

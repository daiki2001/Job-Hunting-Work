#include "Area.h"
#include "Door.h"

namespace
{
static int debugTex = FUNCTION_ERROR;
}

DrawPolygon* Area::draw = nullptr;
BlockManager* Area::block_mgr = BlockManager::Get();
int Area::wall_obj = FUNCTION_ERROR;

Area::Area() :
	door{}
{
	door[DoorNum::UP].Init(Vector3(11.0f, 1.0f, 2.0f), Door::DoorStatus::CLOSE);
	door[DoorNum::DOWN].Init(Vector3(11.0f, 1.0f, 2.0f), Door::DoorStatus::ENTRANCE);
	door[DoorNum::LEFT].Init(Vector3(1.0f, 3.0f, 2.0f), Door::DoorStatus::CLOSE);
	door[DoorNum::RIGHT].Init(Vector3(1.0f, 3.0f, 2.0f), Door::DoorStatus::CLOSE);
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
	block_mgr->Init(Area::draw);
	// 外壁のモデルの読み込み
	wall_obj = Area::draw->CreateOBJModel("./Resources/Game/Wall.obj", "./Resources/Game/");
//#ifdef _DEBUG
	debugTex = Area::draw->LoadTextrue(L"./lib/white1x1.png");
//#endif // _DEBUG
	Door::StaticInit(draw);
}

void Area::Reset()
{
	block_mgr->Reset();
}

void Area::Update()
{
	block_mgr->Update();

	if (block_mgr->GetDoor())
	{
		for (size_t i = 0; i < sizeof(door) / sizeof(door[0]); i++)
		{
			door[i].Open();
		}
	}
}

void Area::Draw(const int& offsetX, const int& offsetY)
{
	const Vector3 offset = Vector3(7.0f, -3.0f, 0.0f) + Vector3(static_cast<float>(offsetX), static_cast<float>(offsetY), 0.0f);

	// 外壁の描画
	DirectDrawing::ChangeMaterialShader();
	draw->DrawOBJ(wall_obj, Vector3(-7.5f, +3.5f, 0.0f) + offset, Math::rotateZ(0 * Math::DEGREE_F * 90.0f), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(-7.5f, -3.5f, 0.0f) + offset, Math::rotateZ(1 * Math::DEGREE_F * 90.0f), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+7.5f, -3.5f, 0.0f) + offset, Math::rotateZ(2 * Math::DEGREE_F * 90.0f), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+7.5f, +3.5f, 0.0f) + offset, Math::rotateZ(3 * Math::DEGREE_F * 90.0f), Vector3(2.0f, 2.0f, 2.0f));

	door[DoorNum::UP].Draw(Vector3(0.0f, +4.5f, 0.0f) + offset);
	door[DoorNum::DOWN].Draw(Vector3(0.0f, -4.5f, 0.0f) + offset);
	door[DoorNum::LEFT].Draw(Vector3(-8.5f, 0.0f, 0.0f) + offset);
	door[DoorNum::RIGHT].Draw(Vector3(+8.5f, 0.0f, 0.0f) + offset);

	block_mgr->Draw(offsetX, -offsetY);
}

void Area::LoadArea(const char* filePath)
{
	static const int stageWidth = 15;
	static const int stageHeight = 7;
	int mapArray[stageWidth * stageHeight] = { BlockManager::TypeId::NONE };

	if (filePath != nullptr)
	{
		Load::LoadMapChip(mapArray, sizeof(mapArray) / sizeof(mapArray[0]), filePath);
	}

	block_mgr->AllDeleteBlock();

	for (int i = 0; i < sizeof(mapArray) / sizeof(mapArray[0]); i++)
	{
		int x = i % stageWidth;
		int y = i / stageWidth;
		int index = block_mgr->CreateBlock(BlockManager::TypeId(mapArray[i]));

		if (index == Engine::FUNCTION_ERROR)
		{
			continue;
		}

		auto& block = block_mgr->GetBlock(index);
		block.pos.x = static_cast<float>(x * 1.0f);
		block.pos.y = static_cast<float>(-y * 1.0f);
	}
}

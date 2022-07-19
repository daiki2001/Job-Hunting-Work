#include "Stage.h"

namespace
{
static int debugTex = FUNCTION_ERROR;
}

DrawPolygon* Stage::draw = nullptr;
BlockManager* Stage::block_mgr = BlockManager::Get();
int Stage::wall_obj = FUNCTION_ERROR;
int Stage::door_obj = FUNCTION_ERROR;

Stage::Stage()
{
}

Stage::~Stage()
{
}

Stage* Stage::Get()
{
	static Stage instance = {};
	return &instance;
}

void Stage::Init(DrawPolygon* const draw)
{
	this->draw = draw;
	block_mgr->Init(draw);
	// 外壁のモデルの読み込み
	wall_obj = draw->CreateOBJModel("./Resources/Game/Wall.obj", "./Resources/Game/");
	door_obj = draw->Create3Dbox(1.0f, 1.0f, 1.0f);
#ifdef _DEBUG
	debugTex = draw->LoadTextrue(L"./lib/white1x1.png");
#endif // _DEBUG
}

void Stage::Update()
{
	block_mgr->Update();
}

void Stage::Draw(const int& offsetX, const int& offsetY)
{
	const Vector3 offset = Vector3(7.0f, -3.0f, 0.0f) + Vector3(static_cast<float>(offsetX), static_cast<float>(offsetY), 0.0f);

	// 外壁の描画
	draw->DrawOBJ(wall_obj, Vector3(-7.5f, +3.5f, 0.0f) + offset, Math::rotateZ(0 * Math::DEGREE_F * 90.0f), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(-7.5f, -3.5f, 0.0f) + offset, Math::rotateZ(1 * Math::DEGREE_F * 90.0f), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+7.5f, -3.5f, 0.0f) + offset, Math::rotateZ(2 * Math::DEGREE_F * 90.0f), Vector3(2.0f, 2.0f, 2.0f));
	draw->DrawOBJ(wall_obj, Vector3(+7.5f, +3.5f, 0.0f) + offset, Math::rotateZ(3 * Math::DEGREE_F * 90.0f), Vector3(2.0f, 2.0f, 2.0f));

	if (block_mgr->GetDoor() == false)
	{
		draw->Draw(door_obj, Vector3(0.0f, +4.5f, 0.0f) + offset, Math::Identity(), Vector3(11.0f, 1.0f, 2.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), debugTex);
	}

	block_mgr->Draw(offsetX, -offsetY);
}

void Stage::LoadStage(const char* filePath)
{
	static const int stageWidth = 15;
	static const int stageHeight = 7;
	int mapArray[stageWidth * stageHeight] = { BlockManager::TypeId::NONE };

	if (filePath != nullptr)
	{
		Load::LoadMapChip(mapArray, sizeof(mapArray) / sizeof(mapArray[0]), filePath);
	}

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
		block.pos.x = static_cast<float>(x);
		block.pos.y = static_cast<float>(-y);
	}
}

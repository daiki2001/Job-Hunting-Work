#include "Stage.h"
#include "./Header/EngineGeneral.h"

BlockManager* Stage::blockMgr = BlockManager::Get();

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
	//this->draw = draw;

	blockMgr->Init(draw);
}

void Stage::Update()
{
	blockMgr->Update();
}

void Stage::Draw(const int& offsetX, const int& offsetY)
{
	blockMgr->Draw(offsetX, offsetY);
}

void Stage::LoadStage(const char* filePath)
{
	static const int stageWidth = 5;
	int mapArray[stageWidth * stageWidth] = { BlockManager::TypeId::NONE };

	if (filePath != nullptr)
	{
		Load::LoadMapChip(mapArray, sizeof(mapArray) / sizeof(mapArray[0]), filePath);
	}

	for (int i = 0; i < sizeof(mapArray) / sizeof(mapArray[0]); i++)
	{
		int x = i % stageWidth;
		int y = i / stageWidth;
		int index = blockMgr->CreateBlock(BlockManager::TypeId(mapArray[i]));

		if (index == Engine::FUNCTION_ERROR)
		{
			continue;
		}

		auto& block = blockMgr->GetBlock(index);
		block.pos.x = static_cast<float>(x * BlockType::WIDTH);
		block.pos.y = static_cast<float>(y * BlockType::HEIGHT);
	}
}

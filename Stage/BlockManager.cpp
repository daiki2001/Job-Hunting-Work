#include "BlockManager.h"

#include "./Header/Error.h"

BlockManager::BlockManager()
{
}

BlockManager::~BlockManager()
{
}

BlockManager* BlockManager::Get()
{
	static BlockManager instance = {};

	return &instance;
}

void BlockManager::Init(DrawPolygon* const draw)
{
	blockType.push_back(BlockType(NONE, draw));
	blockType.back().Create(L"NoneBlock.png");
}

void BlockManager::Update()
{
	for (size_t i = 0; i < block.size(); i++)
	{
		for (size_t j = 0; j < blockType.size(); j++)
		{
			if (block[i].GetTypeId() != blockType[j].GetId())
			{
				continue;
			}
		}
	}
}

void BlockManager::Draw()
{
	for (size_t i = 0; i < block.size(); i++)
	{
		for (size_t j = 0; j < blockType.size(); j++)
		{
			if (block[i].GetTypeId() != blockType[j].GetId())
			{
				continue;
			}

			blockType[j].Draw(block[i].posX, block[i].posY);
			break;
		}
	}
}

int BlockManager::CreateBlock(const BlockManager::TypeId& typeId)
{
	for (size_t i = 0; i < blockType.size(); i++)
	{
		if (typeId != blockType[i].GetId())
		{
			continue;
		}

		block.push_back({});
		block.back().SetTypeId(typeId);
		return static_cast<int>(block.size() - 1);
	}

	return Engine::FUNCTION_ERROR;
}

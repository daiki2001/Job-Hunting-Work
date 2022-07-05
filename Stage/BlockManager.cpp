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

	blockType.push_back(BlockType(WALL, draw));
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

void BlockManager::Draw(const int& offsetX, const int& offsetY)
{
	for (size_t i = 0; i < block.size(); i++)
	{
#ifdef _DEBUG
		DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		switch (block[i].GetTypeId())
		{
		case BlockManager::TypeId::WALL:
			color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		case BlockManager::TypeId::NONE:
		default:
			color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}

		blockType[block[i].GetTypeId()].Draw(block[i].posX + offsetX, block[i].posY + offsetY, color);
#else
		blockType[block[i].GetTypeId()].Draw(block[i].posX + offsetX, block[i].posY + offsetY);
#endif // _DEBUG
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

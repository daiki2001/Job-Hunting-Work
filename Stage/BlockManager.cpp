#include "BlockManager.h"
#include "Player.h"

#include "./Header/Error.h"

namespace
{
static Player* player = Player::Get();
}

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
	blockType.back().Create(L"WallBlock.png");

	blockType.push_back(BlockType(WALL, draw));
	blockType.back().Create(L"WallBlock.png");
}

void BlockManager::Update()
{
	static const int width = 1; //playerSurroundingsBlockの横幅
	static const int size = width * width; //playerSurroundingsBlockの大きさ
	static BlockManager::TypeId playerSurroundingsBlock[size] = {};
	static const int playerBlock = (width - 1) * (width - 1); //プレイヤーがいる場所のブロック

	GetSurroundingBlock(0, playerSurroundingsBlock);

	switch (playerSurroundingsBlock[playerBlock])
	{
	case BlockManager::TypeId::WALL:
		switch (player->GetDirection())
		{
		case Player::Direction::LEFT:
			player->posX++;
			break;
		case Player::Direction::UP:
			player->posY++;
			break;
		case Player::Direction::RIGHT:
			player->posX--;
			break;
		case Player::Direction::DOWN:
			player->posY--;
			break;
		default:
			break;
		}
		break;
	default:
		break;
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

		blockType[block[i].GetTypeId()].Draw(block[i].pos + Vector3(offsetX, offsetY, 0.0f), color);
#else
		blockType[block[i].GetTypeId()].Draw(block[i].pos + Vector3(offsetX, offsetY, 0.0f));
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

void BlockManager::GetSurroundingBlock(const int& radius, BlockManager::TypeId* surroundingBlockType)
{
	const int size = (radius * 2 + 1) * (radius * 2 + 1);
	std::vector<int> surroundingBlock = {};

	for (int i = 0; i < size; i++)
	{
		surroundingBlock.push_back(Engine::FUNCTION_ERROR);
	}

	for (int i = 0, j = 0; i < block.size(); i++)
	{
		if (i == 0)
		{
			for (int y = player->posY - radius; (y < player->posY + radius) && (y < block[i].pos.y / BlockType::HEIGHT); y++)
			{
				for (int x = 0; x < radius * 2 + 1; x++)
				{
					j++;
				}
			}
		}

		if (block[i].pos.x == block[0].pos.x)
		{
			for (int x = player->posX - radius; (x < player->posX + radius) && (x < block[i].pos.x / BlockType::WIDTH); x++)
			{
				j++;
			}
		}

		if (j >= size)
		{
			break;
		}

		if ((block[i].pos.x / BlockType::WIDTH >= player->posX - radius && block[i].pos.x / BlockType::WIDTH <= player->posX + radius) &&
			(block[i].pos.y / BlockType::HEIGHT >= player->posY - radius && block[i].pos.y / BlockType::HEIGHT <= player->posY + radius))
		{
			surroundingBlock[j++] = i;
		}

		if (block[i].pos.x == block[block.size() - 1].pos.x)
		{
			for (int x = player->posX + radius; (x > player->posX - radius) && (x > block[i].pos.x / BlockType::WIDTH); x--)
			{
				j++;
			}
		}

		if (j >= size)
		{
			break;
		}
	}

	for (int i = 0; i < size; i++)
	{
		if (surroundingBlock[i] < 0)
		{
			surroundingBlockType[i] = BlockManager::TypeId::WALL;
			continue;
		}
		surroundingBlockType[i] = block[surroundingBlock[i]].GetTypeId();
	}
}

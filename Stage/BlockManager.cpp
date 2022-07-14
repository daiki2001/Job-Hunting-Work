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
			player->pos.x++;
			break;
		case Player::Direction::UP:
			player->pos.y--;
			break;
		case Player::Direction::RIGHT:
			player->pos.x--;
			break;
		case Player::Direction::DOWN:
			player->pos.y++;
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
		blockType[block[i].GetTypeId()].Draw(block[i].pos + Vector3(static_cast<float>(offsetX), static_cast<float>(offsetY), 0.0f));
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
			for (int y = static_cast<int>(player->pos.y) - radius;
				 (y < static_cast<int>(player->pos.y) + radius) && (y < static_cast<int>(block[i].pos.y));
				 y++)
			{
				for (int x = 0; x < radius * 2 + 1; x++)
				{
					j++;
				}
			}
		}

		if (block[i].pos.x == block[0].pos.x)
		{
			for (int x = static_cast<int>(player->pos.x) - radius;
				 (x < static_cast<int>(player->pos.x) + radius) && (x < static_cast<int>(block[i].pos.x));
				 x++)
			{
				j++;
			}
		}

		if (j >= size)
		{
			break;
		}

		if ((static_cast<int>(block[i].pos.x) >= static_cast<int>(player->pos.x) - radius &&
			 static_cast<int>(block[i].pos.x) <= static_cast<int>(player->pos.x) + radius) &&
			(static_cast<int>(block[i].pos.y) >= static_cast<int>(player->pos.y) - radius &&
			 static_cast<int>(block[i].pos.y) <= static_cast<int>(player->pos.y) + radius))
		{
			surroundingBlock[j++] = i;
		}

		if (block[i].pos.x == block[block.size() - 1].pos.x)
		{
			for (int x = static_cast<int>(player->pos.x) + radius;
				 (x > static_cast<int>(player->pos.x) - radius) && (x > static_cast<int>(block[i].pos.x) / BlockType::WIDTH);
				 x--)
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

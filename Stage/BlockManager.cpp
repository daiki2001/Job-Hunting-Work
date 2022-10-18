#include "BlockManager.h"

Player* BlockManager::player = Player::Get();

BlockManager::Block::Block(TypeId typeId) :
	pos(0.0f, 0.0f, 0.0f),
	typeId(typeId)
{
	if (this->typeId == TypeId::SWITCH)
	{
		this->isSwitch = false;
	}
	else
	{
		this->isSwitch = true;
	}
}

BlockManager::BlockManager() :
	blockType{},
	blocks{},
	isOpen(false),
	isGoal(false),
	keyInitPos{}
{
}

BlockManager::~BlockManager()
{
	blockType.clear();
	blocks.clear();
	keyInitPos.clear();
}

void BlockManager::Init(DrawPolygon* const draw)
{
	blockType.clear();
	blocks.clear();

	blockType.push_back(BlockType(TypeId::NONE, draw));
	blockType.back().Create();

	blockType.push_back(BlockType(TypeId::WALL, draw));
	blockType.back().Create(L"WallBlock.png");

	blockType.push_back(BlockType(TypeId::GOAL, draw));
	blockType.back().Create(L"Goal.png");

	blockType.push_back(BlockType(TypeId::SWITCH, draw));
	blockType.back().Create("Switch.obj");

	blockType.push_back(BlockType(TypeId::KEY, draw));
	blockType.back().Create("key.obj",
							Math::rotateZ(-Math::PI_F / 4),
							scale_xyz(0.25f),
							{ 1.0f, 1.0f, 0.0f, 1.0f });
}

void BlockManager::Update()
{
	static const int width = 1; //playerSurroundingsBlockの横幅
	static const int size = width * width; //playerSurroundingsBlockの大きさ
	static TypeId playerSurroundingsBlock[size] = { TypeId::NONE };
	static const int playerBlock = (width - 1) * (width - 1); //プレイヤーがいる場所のブロック(playerSurroundingsBlock基準)

	const int playerPos = GetSurroundingBlock(0, playerSurroundingsBlock); //プレイヤーがいる場所のブロック

	switch (playerSurroundingsBlock[playerBlock])
	{
	case TypeId::WALL:
		PlayerPushBack();
		break;
	case TypeId::SWITCH:
		SwitchPush(playerPos);
		break;
	case TypeId::GOAL:
		isGoal = true;
		break;
	case TypeId::KEY:
		player->AcquisitionKey();
		keyInitPos.push_back(playerPos);
		blocks[playerPos].typeId = TypeId::NONE;
		break;
	case TypeId::NONE:
	default:
		break;
	}

	isOpen = true;
	for (size_t i = 0; i < blocks.size(); i++)
	{
		if (blocks[i].isSwitch == false)
		{
			isOpen = false;
			break;
		}
	}
}

void BlockManager::Draw(int offsetX, int offsetY)
{
	Vector3 offset = { static_cast<float>(offsetX), static_cast<float>(offsetY), 0.0f };

	for (size_t i = 0; i < blocks.size(); i++)
	{
		blockType[blocks[i].typeId].Draw(blocks[i].pos + offset);
	}
}

void BlockManager::Reset()
{
	for (size_t i = 0; i < blocks.size(); i++)
	{
		if (blocks[i].typeId == TypeId::SWITCH)
		{
			blocks[i].isSwitch = false;
		}
	}

	isGoal = false;

	for (size_t i = 0; i < keyInitPos.size(); i++)
	{
		blocks[keyInitPos[i]].typeId = TypeId::KEY;
	}
	keyInitPos.clear();
}

int BlockManager::CreateBlock(TypeId typeId)
{
	for (size_t i = 0; i < blockType.size(); i++)
	{
		if (typeId != blockType[i].GetId())
		{
			continue;
		}

		blocks.emplace_back(typeId);
		return static_cast<int>(blocks.size() - 1);
	}

	return Engine::FUNCTION_ERROR;
}

void BlockManager::ChengeBlock(int index, TypeId typeId)
{
	Block block = Block(typeId);
	block.pos = blocks[index].pos;
	blocks[index] = block;
}

void BlockManager::DeleteBlock(int index)
{
	blocks.erase(blocks.begin() + index);
}

void BlockManager::AllDeleteBlock()
{
	blocks.clear();
}

void BlockManager::PlayerPushBack() const
{
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
}

void BlockManager::SwitchPush(const size_t& blockNo)
{
	blocks[blockNo].isSwitch = true;
}

int BlockManager::GetSurroundingBlock(int radius, TypeId* surroundingBlockType) const
{
	const int size = (radius * 2 + 1) * (radius * 2 + 1);
	std::vector<int> surroundingBlock = {};
	int playerPos = -1;

	for (int i = 0; i < size; i++)
	{
		surroundingBlock.push_back(Engine::FUNCTION_ERROR);
	}

	for (int i = 0, j = 0; i < static_cast<int>(blocks.size()); i++)
	{
		if (static_cast<int>(blocks[i].pos.x) == static_cast<int>(player->pos.x) &&
			static_cast<int>(blocks[i].pos.y) == static_cast<int>(player->pos.y))
		{
			playerPos = i;
		}

		if (i == 0)
		{
			for (int y = static_cast<int>(player->pos.y) - radius;
				 (y < static_cast<int>(player->pos.y) + radius) && (y < static_cast<int>(blocks[i].pos.y));
				 y++)
			{
				for (int x = 0; x < radius * 2 + 1; x++)
				{
					j++;
				}
			}
		}

		if (blocks[i].pos.x == blocks[0].pos.x)
		{
			for (int x = static_cast<int>(player->pos.x) - radius;
				 (x < static_cast<int>(player->pos.x) + radius) && (x < static_cast<int>(blocks[i].pos.x));
				 x++)
			{
				j++;
			}
		}

		if (j >= size)
		{
			break;
		}

		if ((static_cast<int>(blocks[i].pos.x) >= static_cast<int>(player->pos.x) - radius &&
			 static_cast<int>(blocks[i].pos.x) <= static_cast<int>(player->pos.x) + radius) &&
			(static_cast<int>(blocks[i].pos.y) >= static_cast<int>(player->pos.y) - radius &&
			 static_cast<int>(blocks[i].pos.y) <= static_cast<int>(player->pos.y) + radius))
		{
			surroundingBlock[j++] = i;
		}

		if (blocks[i].pos.x == blocks[blocks.size() - 1].pos.x)
		{
			for (int x = static_cast<int>(player->pos.x) + radius;
				 (x > static_cast<int>(player->pos.x) - radius) && (x > static_cast<int>(blocks[i].pos.x) / BlockType::WIDTH);
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

	if (surroundingBlockType == nullptr)
	{
		return playerPos;
	}

	for (int i = 0; i < size; i++)
	{
		if (surroundingBlock[i] < 0)
		{
			surroundingBlockType[i] = TypeId::WALL;
			continue;
		}
		surroundingBlockType[i] = blocks[surroundingBlock[i]].typeId;
	}

	return playerPos;
}

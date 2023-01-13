#include "BlockManager.h"
#include "Area.h"
#include "./Math/Collision/Collision.h"
#include "./Header/DirectXInit.h"
#include "./Header/Parameter.h"
#include "LoadGraph.h"

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
	itemInitPos{}
{
}

BlockManager::~BlockManager()
{
	blockType.clear();
	blocks.clear();
	itemInitPos.clear();
}

void BlockManager::Init(DrawPolygon* const draw)
{
	blockType.clear();
	blocks.clear();

	blockType.push_back(BlockType(TypeId::NONE, draw));
	blockType.back().Create();

	blockType.push_back(BlockType(TypeId::WALL, draw));
	blockType.back().Create(Parameter::Get(LoadGraph::WALL_BLOCK.c_str()), false);

	blockType.push_back(BlockType(TypeId::GOAL, draw));
	blockType.back().Create(Parameter::Get(LoadGraph::GOAL.c_str()), false);

	blockType.push_back(BlockType(TypeId::SWITCH, draw));
	blockType.back().Create("Switch.obj");

	blockType.push_back(BlockType(TypeId::KEY, draw));
	blockType.back().Create("key.obj",
							Math::rotateZ(-Math::PI_F / 4),
							scale_xyz(0.25f),
							{ 1.0f, 1.0f, 0.0f, 1.0f });

	blockType.push_back(BlockType(TypeId::BOMB, draw));
	blockType.back().Create(Parameter::Get(LoadGraph::BOMB.c_str()), false);
}

void BlockManager::Update()
{
	static const int width = 1; //playerSurroundingsBlockの横幅
	static const int size = width * width; //playerSurroundingsBlockの大きさ
	static TypeId playerSurroundingsBlock[size] = { TypeId::NONE };
	static const int playerBlock = (width - 1) * (width - 1); //プレイヤーがいる場所のブロック(playerSurroundingsBlock基準)

	const int playerPos = GetSurroundingBlock(0, playerSurroundingsBlock); //プレイヤーがいる場所のブロック

	if (playerPos == FUNCTION_ERROR)
	{
		return;
	}

	switch (playerSurroundingsBlock[playerBlock])
	{
	case TypeId::WALL:
		PlayerPushBack(playerPos);
		break;
	case TypeId::SWITCH:
		SwitchPush(playerPos);
		break;
	case TypeId::GOAL:
		isGoal = true;
		break;
	case TypeId::KEY:
		player->AcquisitionKey();
		itemInitPos[playerPos] = TypeId::KEY;
		blocks[playerPos].typeId = TypeId::NONE;
		break;
	case TypeId::BOMB:
		player->AcquisitionBomb();
		itemInitPos[playerPos] = TypeId::BOMB;
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

void BlockManager::Draw(const Vector3& offset)
{
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

	for (auto& i : itemInitPos)
	{
		blocks[i.first].typeId = i.second;
	}
	itemInitPos.clear();
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

	return FUNCTION_ERROR;
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

void BlockManager::PlayerPushBack(int index) const
{
	float a = 0;
	switch (player->GetDirection())
	{
	case Player::Direction::LEFT:
		player->pos.x += 1.0f - (player->pos.x - blocks[index].pos.x);
		break;
	case Player::Direction::UP:
		player->pos.y -= 1.0f + (player->pos.y - blocks[index].pos.y);
		break;
	case Player::Direction::RIGHT:
		player->pos.x -= 1.0f + (player->pos.x - blocks[index].pos.x);
		break;
	case Player::Direction::DOWN:
		player->pos.y += 1.0f - (player->pos.y - blocks[index].pos.y);
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
	Vector3 playerSize = {};

	if ((player->GetDirection() == Player::Direction::UP) || (player->GetDirection() == Player::Direction::DOWN))
	{
		playerSize = Player::COLLISION_SIZE / 2.0f;
	}
	else
	{
		playerSize = Vector3(Player::COLLISION_SIZE.y, Player::COLLISION_SIZE.x, Player::COLLISION_SIZE.z) / 2.0f;
	}

	// 場外判定
	if (Collision::IsAABBToAABBCollision(
		blocks[0].pos - Vector3(0.5f, -0.5f, 0.5f),
		blocks[0].pos + Vector3(0.5f, -0.5f, 0.5f) +
		Vector3(Area::STAGE_WIDTH - 1.0f, -(Area::STAGE_HEIGHT - 1.0f), 0.0f),
		player->pos - playerSize,
		player->pos + playerSize) == false)
	{
		for (int i = 0; i < size; i++)
		{
			if (surroundingBlockType == nullptr)
			{
				break;
			}

			surroundingBlockType[i] = TypeId::WALL;
		}
		return FUNCTION_ERROR;
	}

	for (int i = 0; i < size; i++)
	{
		surroundingBlock.push_back(FUNCTION_ERROR);
	}

	const float playerUp = (player->pos + playerSize - Vector3(1.0f, -1.0f, 0.0f) * static_cast<float>(radius)).y;
	const float playerDown = (player->pos - playerSize + Vector3(1.0f, -1.0f, 0.0f) * static_cast<float>(radius)).y;
	const float playerLeft = (player->pos - playerSize - Vector3(1.0f, -1.0f, 0.0f) * static_cast<float>(radius)).x;
	const float playerRight = (player->pos + playerSize + Vector3(1.0f, -1.0f, 0.0f) * static_cast<float>(radius)).x;

	if ((player->GetDirection() == Player::Direction::UP) || (player->GetDirection() == Player::Direction::LEFT))
	{
		for (int i = 0, j = 0; i < static_cast<int>(blocks.size()); i++)
		{
			// 当たり判定
			if ((playerUp > (blocks[i].pos + Vector3(0.5f, -0.5f, 0.5f)).y &&
				 playerDown < (blocks[i].pos - Vector3(0.5f, -0.5f, 0.5f)).y) &&
				(playerLeft < (blocks[i].pos + Vector3(0.5f, -0.5f, 0.5f)).x &&
				 playerRight >(blocks[i].pos - Vector3(0.5f, -0.5f, 0.5f)).x))
			{
				surroundingBlock[j++] = i;
			}

			if (j >= size)
			{
				break;
			}
		}
	}
	else
	{
		for (int i = static_cast<int>(blocks.size()) - 1, j = 0; i >= 0; i--)
		{
			// 当たり判定
			if ((playerUp > (blocks[i].pos + Vector3(0.5f, -0.5f, 0.5f)).y &&
				 playerDown < (blocks[i].pos - Vector3(0.5f, -0.5f, 0.5f)).y) &&
				(playerLeft < (blocks[i].pos + Vector3(0.5f, -0.5f, 0.5f)).x &&
				 playerRight >(blocks[i].pos - Vector3(0.5f, -0.5f, 0.5f)).x))
			{
				surroundingBlock[j++] = i;
			}

			if (j >= size)
			{
				break;
			}
		}
	}

	for (int i = 0; i < size; i++)
	{
		if (surroundingBlockType != nullptr && surroundingBlock[i] < 0)
		{
			surroundingBlockType[i] = TypeId::WALL;
			continue;
		}

		if (i == size / 2)
		{
			playerPos = surroundingBlock[i];
		}
		if (surroundingBlockType != nullptr)
		{
			surroundingBlockType[i] = blocks[surroundingBlock[i]].typeId;
		}
	}

	return playerPos;
}

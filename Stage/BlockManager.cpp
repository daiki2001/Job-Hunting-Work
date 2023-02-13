﻿#include "BlockManager.h"
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
}

BlockManager::BlockManager() :
	blockType{},
	blocks{},
	isSwitch(false),
	isGoal(false),
	initPos{},
	step(Step::STAY),
	playerInitPos(FUNCTION_ERROR),
	isPlayerMove(false)
{
}

BlockManager::~BlockManager()
{
	blockType.clear();
	blocks.clear();
	initPos.clear();
}

void BlockManager::Init(DrawPolygon* const draw)
{
	BlockType::StaticInit(draw);

	blockType.clear();
	blocks.clear();

	blockType.push_back(BlockType(TypeId::NONE));
	blockType.back().Create();

	blockType.push_back(BlockType(TypeId::WALL));
	blockType.back().Create(Parameter::Get(LoadGraph::WALL_BLOCK.c_str()), false);

	blockType.push_back(BlockType(TypeId::GOAL));
	blockType.back().Create(Parameter::Get(LoadGraph::GOAL.c_str()), false);

	blockType.push_back(BlockType(TypeId::SWITCH));
	blockType.back().Create("Switch.obj");

	blockType.push_back(BlockType(TypeId::KEY));
	blockType.back().Create("key.obj",
							Math::rotateZ(-Math::PI_F / 4),
							scale_xyz(0.25f),
							{ 1.0f, 1.0f, 0.0f, 1.0f });

	blockType.push_back(BlockType(TypeId::BOMB));
	blockType.back().Create(Parameter::Get(LoadGraph::BOMB.c_str()), false);

	blockType.push_back(BlockType(TypeId::MOVE_BLOCK));
	blockType.back().Create(Parameter::Get(LoadGraph::WALL_BLOCK.c_str()), false);

	blockType.push_back(BlockType(TypeId::HOLE));
	blockType.back().Create();
	
	blockType.push_back(BlockType(TypeId::UP_STAIRS));
	blockType.back().Create("Stairs.obj");
	
	blockType.push_back(BlockType(TypeId::DOWN_STAIRS));
	blockType.back().Create();
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
	if(playerInitPos == FUNCTION_ERROR)
	{
		playerInitPos = playerPos;
	}
	isPlayerMove = ((playerPos == playerInitPos) == false);

	switch (playerSurroundingsBlock[playerBlock])
	{
	case TypeId::WALL:
	case TypeId::HOLE:
		PlayerPushBack(playerPos);
		break;
	case TypeId::SWITCH:
		SwitchPush();
		break;
	case TypeId::GOAL:
		isGoal = true;
		break;
	case TypeId::KEY:
		if (player->AcquisitionKey())
		{
			initPos[playerPos] = TypeId::KEY;
			blocks[playerPos].typeId = TypeId::NONE;
		}
		break;
	case TypeId::BOMB:
		if (player->AcquisitionBomb())
		{
			initPos[playerPos] = TypeId::BOMB;
			blocks[playerPos].typeId = TypeId::NONE;
		}
		break;
	case TypeId::MOVE_BLOCK:
		if (GameInput::Get()->DecisionTrigger())
		{
			PushBlock(playerPos);
			initPos[playerPos] = TypeId::MOVE_BLOCK;
		}
		else
		{
			PlayerPushBack(playerPos);
		}
		break;
	case TypeId::UP_STAIRS:
		step = Step::UP;
		break;
	case TypeId::DOWN_STAIRS:
		step = Step::DOWN;
		break;
	case TypeId::NONE:
	default:
		step = Step::STAY;
		break;
	}
}

void BlockManager::Draw(const Vector3& offset)
{
	// ブロックの描画の除外リスト
	static TypeId blockExclude[] = {
		TypeId::NONE,
		TypeId::HOLE,
		TypeId::DOWN_STAIRS,
	};
	// 床の描画の除外リスト
	static TypeId floorExclude[] = {
		TypeId::HOLE,
		TypeId::DOWN_STAIRS,
	};

	bool isSkip = false;
	for (size_t i = 0; i < blocks.size(); i++)
	{
		// ブロックの描画
		isSkip = false;
		for (auto j : blockExclude)
		{
			if (blocks[i].typeId == j)
			{
				isSkip = true;
				break; //ヒットした場合、それ以降はループを回す必要が無い
			}
		}
		if (isSkip == false)
		{
			blockType[blocks[i].typeId].Draw(blocks[i].pos + offset);
		}

		// 床の描画
		isSkip = false;
		for (auto j : floorExclude)
		{
			if (blocks[i].typeId == j)
			{
				isSkip = true;
				break; //ヒットした場合、それ以降はループを回す必要が無い
			}
		}
		if (isSkip == false)
		{
			BlockType::FloorDraw(blocks[i].pos + offset);
		}
	}
}

void BlockManager::Reset()
{
	for (auto& i : initPos)
	{
		blocks[i.first].typeId = i.second;
	}
	initPos.clear();

	for (size_t i = 0; i < blocks.size(); i++)
	{
		if (blocks[i].typeId == TypeId::SWITCH || blocks[i].typeId == TypeId::MOVE_BLOCK)
		{
			isSwitch = false;
			break;
		}
	}

	isGoal = false;
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

void BlockManager::MoveArea()
{
	Vector3 playerSize = {};

	if ((player->GetDirection() == Player::Direction::TOP) || (player->GetDirection() == Player::Direction::BOTTOM))
	{
		playerSize = Player::COLLISION_SIZE / 2.0f;
	}
	else
	{
		playerSize = Vector3(Player::COLLISION_SIZE.y, Player::COLLISION_SIZE.x, Player::COLLISION_SIZE.z) / 2.0f;
	}

	const float playerUp = (player->pos + playerSize - Vector3(1.0f, -1.0f, 0.0f)).y;
	const float playerDown = (player->pos - playerSize + Vector3(1.0f, -1.0f, 0.0f)).y;
	const float playerLeft = (player->pos - playerSize - Vector3(1.0f, -1.0f, 0.0f)).x;
	const float playerRight = (player->pos + playerSize + Vector3(1.0f, -1.0f, 0.0f)).x;

	for (int i = 0; i < static_cast<int>(blocks.size()); i++)
	{
		// 当たり判定
		if ((playerUp > (blocks[i].pos + Vector3(0.5f, -0.5f, 0.5f)).y &&
			 playerDown < (blocks[i].pos - Vector3(0.5f, -0.5f, 0.5f)).y) &&
			(playerLeft < (blocks[i].pos + Vector3(0.5f, -0.5f, 0.5f)).x &&
			 playerRight >(blocks[i].pos - Vector3(0.5f, -0.5f, 0.5f)).x))
		{
			playerInitPos = i;
			break;
		}
	}

	isPlayerMove = false;
}

void BlockManager::PlayerPushBack(int index) const
{
	float a = 0;
	switch (player->GetDirection())
	{
	case Player::Direction::LEFT:
		player->pos.x += 1.0f - (player->pos.x - blocks[index].pos.x);
		break;
	case Player::Direction::RIGHT:
		player->pos.x -= 1.0f + (player->pos.x - blocks[index].pos.x);
		break;
	case Player::Direction::TOP:
		player->pos.y -= 1.0f + (player->pos.y - blocks[index].pos.y);
		break;
	case Player::Direction::BOTTOM:
		player->pos.y += 1.0f - (player->pos.y - blocks[index].pos.y);
		break;
	default:
		break;
	}
}

void BlockManager::SwitchPush()
{
	isSwitch = true;
}

void BlockManager::PushBlock(int index)
{
	int nextBlock = index;

	switch (player->GetDirection())
	{
	case Player::Direction::LEFT:
		if ((index % Area::STAGE_WIDTH) == 0) break;

		nextBlock -= 1;
		break;
	case Player::Direction::RIGHT:
		if ((index % Area::STAGE_WIDTH) == Area::STAGE_WIDTH - 1) break;

		nextBlock += 1;
		break;
	case Player::Direction::TOP:
		if ((index / Area::STAGE_WIDTH) == 0) break;

		nextBlock -= static_cast<int>(Area::STAGE_WIDTH);
		break;
	case Player::Direction::BOTTOM:
		if ((index / Area::STAGE_WIDTH) == 0) break;

		nextBlock += static_cast<int>(Area::STAGE_WIDTH);
		break;
	default:
		break;
	}

	if (nextBlock == index || blocks[nextBlock].typeId != TypeId::NONE)
	{
		return;
	}

	blocks[index].typeId = TypeId::NONE;
	initPos[nextBlock] = blocks[nextBlock].typeId;
	blocks[nextBlock].typeId = TypeId::WALL;
	SwitchPush();
}

int BlockManager::GetSurroundingBlock(int radius, TypeId* surroundingBlockType) const
{
	static const Vector3 halfBlockSize = Vector3(0.5f, -0.5f, 0.5f);
	static const Vector3 LeftBlock = Vector3(-1.0f, 0.0f, 0.0f);
	static const Vector3 RightBlock = Vector3(1.0f, 0.0f, 0.0f);
	static const Vector3 UpBlock = Vector3(0.0f, 1.0f, 0.0f);
	static const Vector3 DownBlock = Vector3(0.0f, -1.0f, 0.0f);

	const int size = (radius * 2 + 1) * (radius * 2 + 1);
	std::vector<int> surroundingBlock = {};
	int playerPos = -1;
	Vector3 playerSize = {};

	if ((player->GetDirection() == Player::Direction::TOP) || (player->GetDirection() == Player::Direction::BOTTOM))
	{
		playerSize = Player::COLLISION_SIZE / 2.0f;
	}
	else
	{
		playerSize = Vector3(Player::COLLISION_SIZE.y, Player::COLLISION_SIZE.x, Player::COLLISION_SIZE.z) / 2.0f;
	}
	playerSize.y *= -1.0f;

	// 場外判定
	if (Collision::IsAABBToAABBCollision(
		blocks[0].pos - halfBlockSize,
		blocks[0].pos + halfBlockSize +
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

	bool isRight = player->GetDirection() == Player::Direction::RIGHT;
	bool isBottom = player->GetDirection() == Player::Direction::BOTTOM;
	// 判定補正
	Vector3 correction = Vector3::Zero();
	correction += LeftBlock * isRight + UpBlock * isBottom;

	for (int i = 0, j = 0; i < static_cast<int>(blocks.size()); i++)
	{
		// 当たり判定
		if (Collision::IsAABBToAABBCollision(
			blocks[i].pos - halfBlockSize + correction,
			blocks[i].pos + halfBlockSize + correction,
			player->pos - playerSize - Vector3(1.0f, -1.0f, 0.0f) * static_cast<float>(radius),
			player->pos + playerSize + Vector3(1.0f, -1.0f, 0.0f) * static_cast<float>(radius)))
		{
			surroundingBlock[j++] = i;
		}

		if (j >= size)
		{
			break;
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

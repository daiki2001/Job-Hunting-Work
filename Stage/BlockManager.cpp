#include "BlockManager.h"
#include "Area.h"
#include "./Math/Collision/Collision.h"
#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/Parameter.h"
#include "LoadGraph.h"

Player* BlockManager::player = Player::Get();
std::vector<BlockType> BlockManager::blockType{};
bool BlockManager::isBlockSwitch = false;
ShaderManager* BlockManager::shaderMgr = ShaderManager::Get();
PostEffect BlockManager::postEffect{};
FireParticle BlockManager::fireEffect{};
int BlockManager::torchLight = FUNCTION_ERROR;

BlockManager::Block::Block(TypeId typeId) :
	pos(0.0f, 0.0f, 0.0f),
	initPos(0.0f, 0.0f, 0.0f),
	typeId(typeId),
	ease{}
{
}

BlockManager::BlockManager() :
	blocks{},
	isSwitch(false),
	isGoal(false),
	initPos{},
	step(Step::STAY),
	playerInitPos(FUNCTION_ERROR),
	isInitPlayer(true),
	isPlayerMove(false)
{
}

BlockManager::~BlockManager()
{
	blocks.clear();
	initPos.clear();
}

void BlockManager::Init()
{
	BlockType::StaticInit();
	fireEffect.Init();

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
							Vector3::Scale_xyz(0.25f),
							Color::AddAlphaValue(Color::YELLOW, 1.0f));

	blockType.push_back(BlockType(TypeId::BOMB));
	blockType.back().Create("Bomb.obj", Math::rotateY(Math::PI_F * 1.5f));

	blockType.push_back(BlockType(TypeId::MOVE_BLOCK));
#if _DEBUG
	blockType.back().Create(Parameter::Get(LoadGraph::WALL_BLOCK.c_str()),
							false,
							Math::Identity(),
							Vector3::Scale_xyz(1.0f),
							Color::AddAlphaValue(Color::YELLOW, 1.0f));
#else
	blockType.back().Create(Parameter::Get(LoadGraph::WALL_BLOCK.c_str()), false, Math::Identity(), Vector3(0.985f, 0.985f, 1.0f));
#endif // !_DEBUG

	blockType.push_back(BlockType(TypeId::SWITCH_BLOCK));
	blockType.back().Create(Parameter::Get(LoadGraph::WALL_BLOCK.c_str()),
							false,
							Math::Identity(),
							Vector3::Scale_xyz(1.0f),
							Color::AddAlphaValue(Color::RED, 1.0f));

	blockType.push_back(BlockType(TypeId::NOT_SWITCH_BLOCK));
	blockType.back().Create(Parameter::Get(LoadGraph::WALL_BLOCK.c_str()),
							false,
							Math::Identity(),
							Vector3::Scale_xyz(1.0f),
							Color::AddAlphaValue(Color::BLUE, 1.0f));

	blockType.push_back(BlockType(TypeId::HOLE));
	blockType.back().Create();
	
	blockType.push_back(BlockType(TypeId::UP_STAIRS));
	blockType.back().Create("Stairs.obj");
	
	blockType.push_back(BlockType(TypeId::DOWN_STAIRS));
	blockType.back().Create();

	blockType.push_back(BlockType(TypeId::WHITE_TILE));
	blockType.back().Create();

	blockType.push_back(BlockType(TypeId::TORCH));
	blockType.back().Create("Torch.obj", Math::rotateX(Math::PI_F), Vector3::Scale_xyz(0.5f));

	blockType.push_back(BlockType(TypeId::BRACELET));
	blockType.back().Create("bracelet.obj", Math::rotateX(Math::PI_F), Vector3::Scale_xyz(0.5f));

	blockType.push_back(BlockType(TypeId::TRANSPARENT_KEY));
	blockType.back().Create();

	if (torchLight == FUNCTION_ERROR)
	{
		postEffect.Init();
		int shader = shaderMgr->CreateShader(StringToWString(shadersDirectory + "PointLightVS.hlsl").c_str(),
											 StringToWString(shadersDirectory + "PointLightPS.hlsl").c_str());
		int gPipeline = shaderMgr->CreateGPipeline(shader, Library::DrawPolygon::GetInstance()->Get3dInputLayout());
		for (size_t i = 0; i < 5; i++)
		{
			shaderMgr->GetGraphicsPipeline(gPipeline, static_cast<ShaderManager::BlendMode>(i)).pRootSignature =
				postEffect.GetRootSignature();
		}
		torchLight = shaderMgr->CreatePipelineState(gPipeline);
	}
}

void BlockManager::EaseInit(vector<Block>& blocks)
{
	Vector3 start = Vector3::Zero();
	Vector3 end = Vector3::Zero();
	float addTime = 0.0f;
	bool isSkip = false;

	for (auto& i : blocks)
	{
		isSkip = false;

		switch (i.typeId)
		{
		case SWITCH_BLOCK:
			start = Vector3(i.pos.x, i.pos.y, 1.0f * (isBlockSwitch == false));
			end = Vector3(i.pos.x, i.pos.y, 1.0f * (isBlockSwitch == true));
			addTime = 0.1f;
			break;
		case NOT_SWITCH_BLOCK:
			start = Vector3(i.pos.x, i.pos.y, 1.0f * (isBlockSwitch == true));
			end = Vector3(i.pos.x, i.pos.y, 1.0f * (isBlockSwitch == false));
			addTime = 0.1f;
			break;
		default:
			isSkip = true;
			break;
		}

		if (isSkip) continue;

		i.ease.isAlive = true;
		i.ease.time = 0.0f;
		i.ease.addTime = addTime;
		i.ease.start = start;
		i.ease.end = end;
	}
}

void BlockManager::Update()
{
	static const int width = 1; //playerSurroundingsBlockの横幅
	static const int size = width * width; //playerSurroundingsBlockの大きさ
	static TypeId playerSurroundingsBlock[size] = { TypeId::NONE };
	static const int playerBlock = (width - 1) * (width - 1); //プレイヤーがいる場所のブロック(playerSurroundingsBlock基準)
	static int playerPos = FUNCTION_ERROR; //プレイヤーがいる場所のブロック
	static int oldPos = FUNCTION_ERROR;

	EffectUpdate();

	oldPos = playerPos;
	playerPos = GetSurroundingBlock(0, playerSurroundingsBlock); //プレイヤーがいる場所のブロック

	if (playerPos == FUNCTION_ERROR)
	{
		return;
	}
	if(isInitPlayer)
	{
		playerInitPos = playerPos;
		isInitPlayer = false;
	}
	isPlayerMove = ((playerPos == playerInitPos) == false);

	int frontBlock = GetPlayerFrontBlock(playerPos);
	if (frontBlock == FUNCTION_ERROR) frontBlock = playerPos;
	if (blocks[frontBlock].typeId == TypeId::MOVE_BLOCK)
	{
		if (GameInput::Get()->DecisionTrigger() && player->GetAction() == Player::SelectItem::BRACELET)
		{
			PushBlock(frontBlock);
		}
		else
		{
			PlayerPushBack(playerPos);
		}
	}
	else
	{
		PlayerPushBack(playerPos);
	}

	switch (playerSurroundingsBlock[playerBlock])
	{
	case TypeId::SWITCH:
		if (playerPos == oldPos) break;

		SwitchPush();
		isBlockSwitch = !isBlockSwitch;
		EaseInit(blocks);
		break;
	case TypeId::GOAL:
		if (playerPos == oldPos) break;

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
	case TypeId::UP_STAIRS:
		step = Step::UP;
		break;
	case TypeId::DOWN_STAIRS:
		step = Step::DOWN;
		break;
	case TypeId::BRACELET:
		if (player->AcquisitionBracelet())
		{
			blocks[playerPos].typeId = TypeId::NONE;
		}
		break;
	case TypeId::NONE:
	case TypeId::WALL:
	case TypeId::MOVE_BLOCK:
	case TypeId::SWITCH_BLOCK:
	case TypeId::NOT_SWITCH_BLOCK:
	case TypeId::HOLE:
	case TypeId::WHITE_TILE:
	case TypeId::TORCH:
	default:
		step = Step::STAY;
		break;
	}
}

void BlockManager::EffectUpdate()
{
	fireEffect.Update();
	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i].typeId == TypeId::TORCH) fireEffect.Create(blocks[i].pos);
		if (blocks[i].ease.isAlive == false) continue;

		blocks[i].ease.time += blocks[i].ease.addTime;
		blocks[i].pos = Math::easeIn(blocks[i].ease.start, blocks[i].ease.end, blocks[i].ease.time);

		if (blocks[i].ease.time >= 1.0f)
		{
			blocks[i].ease.isAlive = false;
			blocks[i].pos = blocks[i].ease.end;
		}
		if (blocks[i].ease.isAlive == false)
		{
			if (blocks[i].typeId == TypeId::MOVE_BLOCK)
			{
				if (blocks[i].pos.z == blocks[i].ease.end.z)
				{
					int index = GetBlock(blocks[i].pos, static_cast<int>(i));
					if (index == FUNCTION_ERROR)
					{
						index = GetBlock(Vector3(blocks[i].pos.x, blocks[i].pos.y, 0.0f), static_cast<int>(i));
						blocks[index].typeId = TypeId::WALL;
						blocks[index].pos = blocks[i].pos;
						blocks[i].typeId = TypeId::NONE;
						blocks[i].pos = blocks[i].initPos;
						SwitchPush();
					}
					else if (blocks[index].typeId == TypeId::NONE ||
							 blocks[index].typeId == TypeId::WHITE_TILE)
					{
						blocks[index].typeId = TypeId::WALL;
						blocks[i].typeId = TypeId::NONE;
						blocks[i].pos = blocks[i].initPos;
						SwitchPush();
					}
					else
					{
						blocks[i].ease.isAlive = true;
						blocks[i].ease.time = 0.0f;
						blocks[i].ease.start = blocks[i].pos;
						blocks[i].ease.end.z += 1.0f;
					}
				}
				else
				{
					blocks[i].typeId = TypeId::NONE;
					blocks[i].pos = blocks[i].initPos;
				}
			}
		}
	}
}

void BlockManager::Draw(const Vector3& offset)
{
	// ブロックの描画の除外リスト
	static TypeId blockExclude[] = {
		TypeId::NONE,
		TypeId::HOLE,
		TypeId::DOWN_STAIRS,
		TypeId::WHITE_TILE,
	};
	// 床の描画の除外リスト
	static TypeId floorExclude[] = {
		TypeId::HOLE,
		TypeId::DOWN_STAIRS,
	};

	bool isSkip = false;
	for (auto& i : blocks)
	{
		// ブロックの描画
		isSkip = false;
		for (auto j : blockExclude)
		{
			if (i.typeId == j)
			{
				isSkip = true;
				break; //ヒットした場合、それ以降はループを回す必要が無い
			}
		}

		if (isSkip == false)
		{
			blockType[i.typeId].Draw(i.pos + offset);
		}

		// 床の描画
		isSkip = false;
		for (auto j : floorExclude)
		{
			if (i.typeId == j)
			{
				isSkip = true;
				break; //ヒットした場合、それ以降はループを回す必要が無い
			}
		}
		if (isSkip == false)
		{
			bool isWhiteTile = (i.typeId == TypeId::WHITE_TILE || i.typeId == TypeId::TRANSPARENT_KEY);
			BlockType::FloorDraw(i.initPos + offset, isWhiteTile);

			if (i.typeId == TypeId::TORCH)
			{
				TorchLight(i.pos + offset);
				fireEffect.Draw(i.pos + offset);
			}
		}
	}
}

void BlockManager::Reset()
{
	for (auto& i : initPos)
	{
		blocks[i.first].typeId = i.second;
		blocks[i.first].pos = blocks[i.first].initPos;
	}
	initPos.clear();

	for (auto& i : blocks)
	{
		if (i.typeId == TypeId::SWITCH || i.typeId == TypeId::MOVE_BLOCK)
		{
			isSwitch = false;
			break;
		}
	}

	playerInitPos = FUNCTION_ERROR;
	isBlockSwitch = false;
	isGoal = false;
}

void BlockManager::MapInit()
{
	static TypeId resetType[] = {
		TypeId::NONE,
		TypeId::BOMB,
		TypeId::MOVE_BLOCK,
		TypeId::HOLE,
		TypeId::WHITE_TILE,
	};

	for (auto i = initPos.begin(); i != initPos.end();)
	{
		bool isInit = false;
		for (auto j : resetType)
		{
			if (i->second == j)
			{
				isInit = true;
				break;
			}
		}

		if (isInit)
		{
			blocks[i->first].typeId = i->second;
			blocks[i->first].pos = blocks[i->first].initPos;
			i = initPos.erase(i);
		}
		else
		{
			i++;
		}
	}
}

int BlockManager::CreateBlock(TypeId typeId)
{
	for (auto& i : blockType)
	{
		if (typeId != i.GetId())
		{
			continue;
		}

		blocks.emplace_back(typeId);
		if (blocks.back().typeId == NOT_SWITCH_BLOCK)
		{
			blocks.back().pos.z = 1.0f;
		}
		return static_cast<int>(blocks.size() - 1);
	}

	return FUNCTION_ERROR;
}

void BlockManager::ChengeBlock(int index, TypeId typeId)
{
	Block block = Block(typeId);
	block.pos = blocks[index].pos;

	if (block.typeId == NOT_SWITCH_BLOCK)
	{
		block.pos.z = 1.0f;
	}
	else
	{
		block.pos.z = 0.0f;
	}
	block.initPos = block.pos;

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
	static TypeId hitBlock[] =
	{
		TypeId::WALL,
		TypeId::MOVE_BLOCK,
		TypeId::SWITCH_BLOCK,
		TypeId::NOT_SWITCH_BLOCK,
		TypeId::HOLE,
		TypeId::TORCH,
	};

	int frontBlock = index; //自機から見て前にあるブロックのインデックス
	int frontLeftBlock = index; //自機から見て左前にあるブロックのインデックス
	int frontRightBlock = index; //自機から見て右前にあるブロックのインデックス
	Vector3 pushDistanse = Vector3::Zero();
	switch (player->GetDirection())
	{
	case Player::Direction::LEFT:
		frontBlock -= 1;
		if (frontBlock < 0) return; //場外判定

		// 自機から見て斜め前にあるブロックの位置の計算
		frontLeftBlock = frontBlock + STAGE_WIDTH;
		frontRightBlock = frontBlock - STAGE_WIDTH;
		if (frontLeftBlock / STAGE_WIDTH >= STAGE_HEIGHT) frontLeftBlock = FUNCTION_ERROR;
		if (frontRightBlock < 0) frontRightBlock = FUNCTION_ERROR;

		pushDistanse.x += 1.0f - (player->pos.x - blocks[frontBlock].pos.x);
		break;
	case Player::Direction::RIGHT:
		// 自機から見て斜め前にあるブロックの位置の計算
		frontLeftBlock = frontBlock - STAGE_WIDTH;
		frontRightBlock = frontBlock + STAGE_WIDTH;
		if (frontLeftBlock < 0) frontLeftBlock = FUNCTION_ERROR;
		if (frontRightBlock / STAGE_WIDTH >= STAGE_HEIGHT) frontRightBlock = FUNCTION_ERROR;

		pushDistanse.x -= 1.0f + (player->pos.x - blocks[frontBlock].pos.x);
		break;
	case Player::Direction::TOP:
		frontBlock -= STAGE_WIDTH;
		if (frontBlock < 0) return; //場外判定

		// 自機から見て斜め前にあるブロックの位置の計算
		frontLeftBlock = frontBlock - 1;
		frontRightBlock = frontBlock + 1;
		if (frontLeftBlock < 0) frontLeftBlock = FUNCTION_ERROR;
		if (frontRightBlock >= blocks.size()) frontRightBlock = FUNCTION_ERROR;

		pushDistanse.y -= 1.0f + (player->pos.y - blocks[frontBlock].pos.y);
		break;
	case Player::Direction::BOTTOM:
		// 自機から見て斜め前にあるブロックの位置の計算
		frontLeftBlock = frontBlock + 1;
		frontRightBlock = frontBlock - 1;
		if (frontLeftBlock >= blocks.size()) frontLeftBlock = FUNCTION_ERROR;
		if (frontRightBlock < 0) frontRightBlock = FUNCTION_ERROR;

		pushDistanse.y += 1.0f - (player->pos.y - blocks[frontBlock].pos.y);
		break;
	default:
		break;
	}

	bool isHitBlock = false;
	bool frontBlockIsHit = false; //自機から見て前にあるブロックが壁かどうか
	bool frontLeftBlockIsHit = false; //自機から見て左前にあるブロックが壁かどうか
	bool frontRightBlockIsHit = false; //自機から見て右前にあるブロックが壁かどうか

	for (auto i : hitBlock)
	{
		frontBlockIsHit |= (i == blocks[frontBlock].typeId);
		frontLeftBlockIsHit |=
			(frontLeftBlock != FUNCTION_ERROR && i == blocks[frontLeftBlock].typeId);
		frontRightBlockIsHit |=
			(frontRightBlock != FUNCTION_ERROR && i == blocks[frontRightBlock].typeId);

		if (frontBlockIsHit || frontLeftBlockIsHit || frontRightBlockIsHit)
		{
			isHitBlock = true;
		}
	}

	if (isHitBlock == false) return;

	static const Vector3 halfBlockSize = Vector3(0.5f, -0.5f, 0.5f);
	static const Vector3 leftBlock = Vector3(-1.0f, 0.0f, 0.0f);
	static const Vector3 rightBlock = Vector3(1.0f, 0.0f, 0.0f);
	static const Vector3 upBlock = Vector3(0.0f, 1.0f, 0.0f);
	static const Vector3 downBlock = Vector3(0.0f, -1.0f, 0.0f);

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

	bool isRight = player->GetDirection() == Player::Direction::RIGHT;
	bool isBottom = player->GetDirection() == Player::Direction::BOTTOM;
	Vector3 correction = leftBlock * isRight + upBlock * isBottom; //判定補正
	
	// 当たり判定
	bool isHit = frontBlockIsHit && 
		Collision::IsAABBToAABBCollision(
			blocks[frontBlock].pos - halfBlockSize + correction,
			blocks[frontBlock].pos + halfBlockSize + correction,
			player->pos - playerSize,
			player->pos + playerSize);
	isHit |= frontLeftBlockIsHit && 
		Collision::IsAABBToAABBCollision(
			blocks[frontLeftBlock].pos - halfBlockSize + correction,
			blocks[frontLeftBlock].pos + halfBlockSize + correction,
			player->pos - playerSize,
			player->pos + playerSize);
	isHit |= frontRightBlockIsHit && 
		Collision::IsAABBToAABBCollision(
			blocks[frontRightBlock].pos - halfBlockSize + correction,
			blocks[frontRightBlock].pos + halfBlockSize + correction,
			player->pos - playerSize,
			player->pos + playerSize);
	if (isHit)
	{
		player->pos += pushDistanse;
	}
}

void BlockManager::SwitchPush()
{
	isSwitch = true;

	for (auto& i : blocks)
	{
		if (i.typeId != TypeId::TRANSPARENT_KEY) continue;

		i.typeId = TypeId::KEY;
	}
}

void BlockManager::PushBlock(int index)
{
	int nextBlock = index;

	switch (player->GetDirection())
	{
	case Player::Direction::LEFT:
		if ((index % STAGE_WIDTH) == 0) break;

		nextBlock -= 1;
		break;
	case Player::Direction::RIGHT:
		if ((index % STAGE_WIDTH) == STAGE_WIDTH - 1) break;

		nextBlock += 1;
		break;
	case Player::Direction::TOP:
		if ((index / STAGE_WIDTH) == 0) break;

		nextBlock -= static_cast<int>(STAGE_WIDTH);
		break;
	case Player::Direction::BOTTOM:
		if ((index / STAGE_WIDTH) == STAGE_HEIGHT - 1) break;

		nextBlock += static_cast<int>(STAGE_WIDTH);
		break;
	default:
		break;
	}

	if (nextBlock == index)
	{
		return;
	}
	if (blocks[nextBlock].typeId != TypeId::NONE && blocks[nextBlock].typeId != TypeId::HOLE && blocks[nextBlock].typeId != TypeId::WHITE_TILE)
	{
		return;
	}

	if (initPos.find(index) == initPos.end()) initPos[index] = TypeId::MOVE_BLOCK;
	if (initPos.find(nextBlock) == initPos.end()) initPos[nextBlock] = blocks[nextBlock].typeId;

	blocks[index].ease.isAlive = true;
	blocks[index].ease.time = 0.0f;
	blocks[index].ease.addTime = 0.05f;
	blocks[index].ease.start = blocks[index].pos;
	blocks[index].ease.end = blocks[nextBlock].pos;
}

void BlockManager::TorchLight(const Vector3& pos)
{
	auto draw = Library::DrawPolygon::GetInstance();
	static int plane = draw->CreateCircle(0.5f, 8);
	static int graph = draw->LoadTextrue(L"./Resources/CircleBlur.png");

	draw->SetDrawBlendMode(ShaderManager::BlendMode::ALPHA);
	draw->ChangeOBJShader();
	draw->Draw(plane, pos + Vector3(0.0f, 0.0f, -0.5f), Math::Identity(), Vector3::Scale_xyz(1.0f), Color::AddAlphaValue(Color::ORANGE, 1.0f), graph, true);
	draw->SetDrawBlendMode(ShaderManager::BlendMode::ALPHA);
}

int BlockManager::GetBlock(const Vector3& pos, int skipIndex)
{
	int result = FUNCTION_ERROR;
	int posX = static_cast<int>(Math::RoundOff(pos.x));
	int posY = static_cast<int>(Math::RoundOff(pos.y));
	if (posX < 0 || posX >= STAGE_WIDTH || posY > 0 || posY <= static_cast<int>(STAGE_HEIGHT) * -1.0f)
	{
		return result;
	}

	for (int i = 0; i < static_cast<int>(blocks.size()); i++)
	{
		if (i == skipIndex) continue;

		if (Vector3(static_cast<float>(posX), static_cast<float>(posY), pos.z) == blocks[i].initPos)
		{
			result = i;
		}
	}

	return result;
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

	// 場外判定
	if (Collision::IsAABBToAABBCollision(
		blocks[0].pos - halfBlockSize,
		blocks[0].pos + halfBlockSize +
		Vector3(STAGE_WIDTH - 1.0f, -(STAGE_HEIGHT - 1.0f), 0.0f),
		player->pos,
		player->pos) == false)
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
			player->pos - Vector3(1.0f, -1.0f, 0.0f) * static_cast<float>(radius),
			player->pos + Vector3(1.0f, -1.0f, 0.0f) * static_cast<float>(radius)))
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

int BlockManager::GetPlayerFrontBlock(int playerPos) const
{
	int result = playerPos;

	switch (player->GetDirection())
	{
	case Player::Direction::LEFT:
		result -= 1;
		if (result < 0) return FUNCTION_ERROR; //場外判定
		break;
	case Player::Direction::TOP:
		result -= STAGE_WIDTH;
		if (result < 0) return FUNCTION_ERROR; //場外判定
		break;
	case Player::Direction::RIGHT:
	case Player::Direction::BOTTOM:
	default:
		break;
	}

	return result;
}

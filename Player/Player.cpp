#include "Player.h"
#include "./Stage/BlockType.h"
#include "./Stage/Stage.h"
#include "./Math/Collision/Collision.h"
#include "./Header/Parameter.h"
#include "LoadGraph.h"

namespace
{
static Stage* stage = Stage::Get();
}

const float Player::SPEED = 0.3f;
const Math::Vector3 Player::COLLISION_SIZE = Vector3(0.25f, 1.0f, 1.0f);

Player::Player() :
	draw(nullptr),
	pos{},
	direction(Player::Direction::UP),
	object(Engine::FUNCTION_ERROR),
	selectItem(SelectItem::KEY),
	key(100),
	bomb(100, 10),
	route{}
{
	Reset();
}

Player::~Player()
{
}

Player* Player::Get()
{
	static Player instance = {};
	return &instance;
}

void Player::Init(DrawPolygon* const draw)
{
	this->draw = draw;
	object = this->draw->CreateOBJModel("./Resources/Game/Player.obj", "./Resources/Game/");

	Item::StaticInit(this->draw);
	key.Init(Parameter::Get(LoadGraph::KEY.c_str()));
	bomb.Init();
	route.reserve(Area::MAX_COURSE_NUM);
}

void Player::Update(const GameInput* const input)
{
	bomb.Update();

	Move(input);
	SelectAction(input);
	Action(input);
}

void Player::Draw(int offsetX, int offsetY)
{
	bomb.Draw();

	DirectDrawing::ChangeMaterialShader();
	draw->DrawOBJ(
		object,
		pos + Vector3(static_cast<float>(offsetX),
					  static_cast<float>(offsetY),
					  0.0f),
		Math::rotateZ(direction * Math::DEGREE_F * 90.0f),
		scale_xyz(1.0f / 256.0f),
		DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

void Player::DrawInventory(int offsetX, int offsetY, float scale)
{
	switch (selectItem)
	{
	case Player::KEY:
		key.DrawInfo("Key", offsetX, offsetY, scale);
		break;
	case Player::BOMB:
		bomb.DrawInfo("Bomb", offsetX, offsetY, scale);
		break;
	default:
		break;
	}
}

void Player::Reset()
{
	pos = { 7.0f, -6.0f, 0.0f };
	selectItem = SelectItem::KEY;
	key.Reset();
	bomb.Reset();
}

void Player::Move(const GameInput* const input)
{
	if (input->MainUp())
	{
		MoveUp(input);
	}
	else if (input->MainDown())
	{
		MoveDown(input);
	}
	else if (input->MainLeft())
	{
		MoveLeft(input);
	}
	else if (input->MainRight())
	{
		MoveRight(input);
	}
}

void Player::SelectAction(const GameInput* const input)
{
	if (input->SubLeftTrigger())
	{
		selectItem = (selectItem - 1 < 0) ? static_cast<SelectItem>(0) : static_cast<SelectItem>(selectItem - 1);
	}
	else if (input->SubRightTrigger())
	{
		selectItem = (selectItem + 1 >= SelectItem::MAX) ? static_cast<SelectItem>(SelectItem::MAX - 1) : static_cast<SelectItem>(selectItem + 1);
	}
}

void Player::Action(const GameInput* const input)
{
	if (input->DecisionTrigger())
	{
		switch (selectItem)
		{
		case SelectItem::KEY:
			KeyAction();
			break;
		case SelectItem::BOMB:
			BombAction();
			break;
		default:
			break;
		}
	}
}

void Player::MoveUp(const GameInput* const input)
{
	direction = Player::Direction::UP;

	if (pos.y > 0.0f)
	{
		return;
	}

	pos.y += SPEED;

	if (((pos.x <= 8.0f && pos.x >= 6.0f) &&
		 (pos.y >= 0.0f)) &&
		(stage->GetDoorStatus(Area::DoorNum::UP) == Door::DoorStatus::OPEN))
	{
		route.emplace_back(Area::DoorNum::UP);

		switch (stage->GetArea().LostForest(route))
		{
		case 0:
			if (stage->MoveUpRoom() == 0)
			{
				route.clear();
			}
			break;
		case 2:
		case FUNCTION_ERROR:
			route.clear();
			route.emplace_back(Area::DoorNum::UP);
			break;
		default:
			break;
		}

		pos.y = -(static_cast<int>(Area::STAGE_HEIGHT) - 1.0f);
	}

	// 場外判定
	if ((pos + COLLISION_SIZE / 2.0f).y > stage->GetBlockManager()->GetBlock(0).pos.y)
	{
		pos.y = 0.0f;
	}
}

void Player::MoveDown(const GameInput* const input)
{
	direction = Player::Direction::DOWN;

	if (pos.y < -(Area::STAGE_HEIGHT - 1.0f))
	{
		return;
	}

	pos.y -= SPEED;

	if (((pos.x <= 8.0f && pos.x >= 6.0f) &&
		 (pos.y < -(Area::STAGE_HEIGHT - 1.0f))) &&
		(stage->GetDoorStatus(Area::DoorNum::DOWN) == Door::DoorStatus::OPEN))
	{
		route.emplace_back(Area::DoorNum::DOWN);

		switch (stage->GetArea().LostForest(route))
		{
		case 0:
			if (stage->MoveDownRoom() == 0)
			{
				route.clear();
			}
			break;
		case 2:
		case FUNCTION_ERROR:
			route.clear();
			route.emplace_back(Area::DoorNum::DOWN);
			break;
		default:
			break;
		}

		pos.y = 0.0f;
	}

	// 場外判定
	if ((pos - COLLISION_SIZE / 2.0f).y < stage->GetBlockManager()->GetBlock(0).pos.y - (Area::STAGE_HEIGHT - 1.0f))
	{
		pos.y = -(Area::STAGE_HEIGHT - 1.0f);
	}
}

void Player::MoveLeft(const GameInput* const input)
{
	direction = Player::Direction::LEFT;

	if (pos.x < 0.0f)
	{
		return;
	}

	pos.x -= SPEED;

	if (((pos.x < 0.0f)) &&
		 (pos.y <= -2.0f && pos.y >= -4.0f) &&
		(stage->GetDoorStatus(Area::DoorNum::LEFT) == Door::DoorStatus::OPEN))
	{
		route.emplace_back(Area::DoorNum::LEFT);

		switch (stage->GetArea().LostForest(route))
		{
		case 0:
			if (stage->MoveLeftRoom() == 0)
			{
				route.clear();
			}
			break;
		case 2:
		case FUNCTION_ERROR:
			route.clear();
			route.emplace_back(Area::DoorNum::LEFT);
			break;
		default:
			break;
		}

		pos.x = Area::STAGE_WIDTH - 1.0f;
	}

	static const Vector3 COLLISION_BOX = Vector3(COLLISION_SIZE.y, COLLISION_SIZE.x, COLLISION_SIZE.z) / 2.0f;

	// 場外判定
	if ((pos - COLLISION_BOX).x < stage->GetBlockManager()->GetBlock(0).pos.x)
	{
		pos.x = 0.0f;
	}
}

void Player::MoveRight(const GameInput* const input)
{
	direction = Player::Direction::RIGHT;

	if (pos.x > Area::STAGE_WIDTH - 1.0f)
	{
		return;
	}

	pos.x += SPEED;

	if (((pos.x > Area::STAGE_WIDTH - 1.0f) && 
		 (pos.y <= -2.0f && pos.y >= -4.0f)) &&
		(stage->GetDoorStatus(Area::DoorNum::RIGHT) == Door::DoorStatus::OPEN))
	{
		route.emplace_back(Area::DoorNum::RIGHT);

		switch (stage->GetArea().LostForest(route))
		{
		case 0:
			if (stage->MoveRightRoom() == 0)
			{
				route.clear();
			}
			break;
		case 2:
		case FUNCTION_ERROR:
			route.clear();
			route.emplace_back(Area::DoorNum::RIGHT);
			break;
		default:
			break;
		}

		pos.x = 0.0f;
	}

	static const Vector3 COLLISION_BOX = Vector3(COLLISION_SIZE.y, COLLISION_SIZE.x, COLLISION_SIZE.z) / 2.0f;

	// 場外判定
	if ((pos + COLLISION_BOX).x > stage->GetBlockManager()->GetBlock(0).pos.x + (Area::STAGE_WIDTH - 1.0f))
	{
		pos.x = Area::STAGE_WIDTH - 1.0f;
	}
}

void Player::KeyAction()
{
	if (key.GetCount() <= 0) return;

	int playerPos = stage->GetBlockManager()->GetSurroundingBlock(0, nullptr);

	if ((playerPos == FUNCTION_ERROR ||
		 (playerPos % 15 == 0 && playerPos / 15 >= 2 && playerPos / 15 <= 4)) &&
		stage->GetDoorStatus(Area::DoorNum::LEFT) == Door::DoorStatus::KEY_CLOSE)
	{
		stage->GetArea().GetDoor(Area::DoorNum::LEFT).KeyOpen();
		key.Use();
	}
	if ((playerPos == FUNCTION_ERROR ||
		 (playerPos % 15 == 14 && playerPos / 15 >= 2 && playerPos / 15 <= 4)) &&
		stage->GetDoorStatus(Area::DoorNum::RIGHT) == Door::DoorStatus::KEY_CLOSE)
	{
		stage->GetArea().GetDoor(Area::DoorNum::RIGHT).KeyOpen();
		key.Use();
	}
	if ((playerPos == FUNCTION_ERROR ||
		 (playerPos >= 6 && playerPos <= 8)) &&
		stage->GetDoorStatus(Area::DoorNum::UP) == Door::DoorStatus::KEY_CLOSE)
	{
		stage->GetArea().GetDoor(Area::DoorNum::UP).KeyOpen();
		key.Use();
	}
	if ((playerPos == FUNCTION_ERROR ||
		 (playerPos >= 96 && playerPos <= 98)) &&
		stage->GetDoorStatus(Area::DoorNum::DOWN) == Door::DoorStatus::KEY_CLOSE)
	{
		stage->GetArea().GetDoor(Area::DoorNum::DOWN).KeyOpen();
		key.Use();
	}
}

void Player::BombAction()
{
	if (bomb.GetCount() <= 0) return;

	bomb.Set(pos);
}

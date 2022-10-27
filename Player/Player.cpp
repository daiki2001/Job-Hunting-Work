#include "Player.h"
#include "./Stage/BlockType.h"
#include "./Stage/Stage.h"
#include "./Math/Collision/Collision.h"
#include "./Header/Parameter.h"

const float Player::SPEED = 0.3f;
const Math::Vector3 Player::COLLISION_SIZE = Vector3(0.25f, 1.0f, 1.0f);

Player::Player() :
	draw(nullptr),
	pos{},
	direction(Player::Direction::UP),
	object(Engine::FUNCTION_ERROR),
	keyCount(0)
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
	//object = this->draw->Create3Dbox(COLLISION_SIZE);
}

void Player::Update(const InputManager* const input)
{
	Move(input);
}

void Player::Draw(int offsetX, int offsetY)
{
	//DirectDrawing::ChangeOBJShader();
	//draw->Draw(
	//	object,
	//	pos + Vector3(static_cast<float>(offsetX),
	//				  static_cast<float>(offsetY),
	//				  0.0f),
	//	Math::rotateZ(direction * Math::DEGREE_F * 90.0f),
	//	scale_xyz(1.0f),
	//	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	//	Parameter::Get("white1x1")
	//);
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

void Player::Reset()
{
	pos = { 7.0f, -6.0f, 0.0f };
	keyCount = 0;
}

void Player::Move(const InputManager* const input)
{
	if (input->MainUp() || input->SubUp())
	{
		MoveUp(input);
	}
	else if (input->MainDown() || input->SubDown())
	{
		MoveDown(input);
	}
	else if (input->MainLeft() || input->SubLeft())
	{
		MoveLeft(input);
	}
	else if (input->MainRight() || input->SubRight())
	{
		MoveRight(input);
	}
}

void Player::MoveUp(const InputManager* const input)
{
	direction = Player::Direction::UP;

	if (pos.y > 0.0f) return;

	pos.y += SPEED;

	if ((pos.x <= 8.0f && pos.x >= 6.0f) && pos.y >= 0.0f)
	{
		if (Stage::GetDoorStatus(Area::DoorNum::UP) == Door::DoorStatus::OPEN)
		{
			if (Stage::MoveUpRoom() == 0)
			{
				pos.y = -(Area::STAGE_HEIGHT - 1.0f);
			}
		}
	}

	// 場外判定
	if ((pos + COLLISION_SIZE / 2.0f).y > Stage::GetBlockManager()->GetBlock(0).pos.y)
	{
		pos.y = 0.0f;
	}
}

void Player::MoveDown(const InputManager* const input)
{
	direction = Player::Direction::DOWN;

	if (pos.y < -(Area::STAGE_HEIGHT - 1.0f))
	{
		return;
	}

	pos.y -= SPEED;

	if ((pos.x <= 8.0f && pos.x >= 6.0f) && pos.y < -(Area::STAGE_HEIGHT - 1.0f))
	{
		if (Stage::GetDoorStatus(Area::DoorNum::DOWN) == Door::DoorStatus::OPEN)
		{
			if (Stage::MoveDownRoom() == 0)
			{
				pos.y = 0.0f;
			}
		}
	}

	// 場外判定
	if ((pos - COLLISION_SIZE / 2.0f).y < Stage::GetBlockManager()->GetBlock(0).pos.y - (Area::STAGE_HEIGHT - 1.0f))
	{
		pos.y = -(Area::STAGE_HEIGHT - 1.0f);
	}
}

void Player::MoveLeft(const InputManager* const input)
{
	direction = Player::Direction::LEFT;

	if (pos.x < 0.0f)
	{
		return;
	}

	pos.x -= SPEED;

	if ((pos.y <= -2.0f && pos.y >= -4.0f) && pos.x < 0.0f)
	{
		if (Stage::GetDoorStatus(Area::DoorNum::LEFT) == Door::DoorStatus::OPEN)
		{
			if (Stage::MoveLeftRoom() == 0)
			{
				pos.x = Area::STAGE_WIDTH - 1.0f;
			}
		}
	}

	static const Vector3 COLLISION_BOX = Vector3(COLLISION_SIZE.y, COLLISION_SIZE.x, COLLISION_SIZE.z) / 2.0f;

	// 場外判定
	if ((pos - COLLISION_BOX).x < Stage::GetBlockManager()->GetBlock(0).pos.x)
	{
		pos.x = 0.0f;
	}
}

void Player::MoveRight(const InputManager* const input)
{
	direction = Player::Direction::RIGHT;

	if (pos.x > Area::STAGE_WIDTH - 1.0f)
	{
		return;
	}

	pos.x += SPEED;

	if ((pos.y <= -2.0f && pos.y >= -4.0f) && pos.x > Area::STAGE_WIDTH - 1.0f)
	{
		if (Stage::GetDoorStatus(Area::DoorNum::RIGHT) == Door::DoorStatus::OPEN)
		{
			if (Stage::MoveRightRoom() == 0)
			{
				pos.x = 0.0f;
			}
		}
	}

	static const Vector3 COLLISION_BOX = Vector3(COLLISION_SIZE.y, COLLISION_SIZE.x, COLLISION_SIZE.z) / 2.0f;

	// 場外判定
	if ((pos + COLLISION_BOX).x > Stage::GetBlockManager()->GetBlock(0).pos.x + (Area::STAGE_WIDTH - 1.0f))
	{
		pos.x = Area::STAGE_WIDTH - 1.0f;
	}
}

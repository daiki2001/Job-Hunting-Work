#include "Player.h"
#include "./Stage/BlockType.h"
#include "./Stage/Stage.h"

const float Player::SPEED = 0.3f;

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
}

void Player::Update(const InputManager* const input)
{
	Move(input);
}

void Player::Draw(int offsetX, int offsetY)
{
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
		if (pos.y <= 0.0f) pos.y += SPEED;
		direction = Player::Direction::UP;

		if ((pos.x <= 8.0f && pos.x >= 6.0f) && pos.y > 0.0f)
		{
			if (Stage::GetDoorStatus(Area::DoorNum::UP) == Door::DoorStatus::OPEN)
			{
				if (Stage::MoveUpRoom() == 0)
				{
					pos.y = -(Area::STAGE_HEIGHT - 1.0f);
				}
			}
		}
	}
	else if (input->MainDown() || input->SubDown())
	{
		if (pos.y >= -(Area::STAGE_HEIGHT - 1.0f)) pos.y -= SPEED;
		direction = Player::Direction::DOWN;

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
	}
	else if (input->MainLeft() || input->SubLeft())
	{
		if (pos.x >= 0.0f) pos.x -= SPEED;
		direction = Player::Direction::LEFT;

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
	}
	else if (input->MainRight() || input->SubRight())
	{
		if (pos.x <= Area::STAGE_WIDTH - 1.0f) pos.x += SPEED;
		direction = Player::Direction::RIGHT;

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
	}
}

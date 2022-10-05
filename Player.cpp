#include "Player.h"
#include "./Stage/BlockType.h"
#include "./Stage/Stage.h"

Player::Player() :
	draw(nullptr),
	pos{},
	direction(Player::Direction::UP),
	object(Engine::FUNCTION_ERROR),
	graph(Engine::FUNCTION_ERROR)
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
	object = this->draw->CreateCylinder(0.5f, 0.5f, 3);
	graph = this->draw->LoadTextrue(L"./Resources/Game/Player.png");
}

void Player::Update(const InputManager* const input)
{
	Move(input);
}

void Player::Draw(const int& offsetX, const int& offsetY)
{
	DirectDrawing::ChangeOBJShader();
	draw->Draw(
		object,
		pos + Vector3(static_cast<float>(offsetX),
					  static_cast<float>(offsetY),
					  0.0f),
		Math::rotateZ(direction * Math::DEGREE_F * 90.0f),
		{ 0.5f, 1.0f, 1.0f },
		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		graph
	);
}

void Player::Reset()
{
	pos = { 7.0f, -6.0f, 0.0f };
}

void Player::Move(const InputManager* const input)
{
	if (input->MainUpTrigger() || input->SubUpTrigger())
	{
		pos.y += 1.0f;
		direction = Player::Direction::UP;

		if ((pos.x <= 8.0f && pos.x >= 6.0f) && pos.y > 0.0f)
		{
			if (Stage::GetDoorStatus(Area::DoorNum::UP) == Door::DoorStatus::OPEN)
			{
				if (Stage::MoveUpRoom() == 0)
				{
					pos.y = -6.0f;
				}
			}
		}
	}
	else if (input->MainDownTrigger() || input->SubDownTrigger())
	{
		pos.y -= 1.0f;
		direction = Player::Direction::DOWN;

		if ((pos.x <= 8.0f && pos.x >= 6.0f) && pos.y < -6.0f)
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
	else if (input->MainLeftTrigger() || input->SubLeftTrigger())
	{
		pos.x -= 1.0f;
		direction = Player::Direction::LEFT;

		if ((pos.y <= -2.0f && pos.y >= -4.0f) && pos.x < 0.0f)
		{
			if (Stage::GetDoorStatus(Area::DoorNum::LEFT) == Door::DoorStatus::OPEN)
			{
				if (Stage::MoveLeftRoom() == 0)
				{
					pos.x = 14.0f;
				}
			}
		}
	}
	else if (input->MainRightTrigger() || input->SubRightTrigger())
	{
		pos.x += 1.0f;
		direction = Player::Direction::RIGHT;

		if ((pos.y <= -2.0f && pos.y >= -4.0f) && pos.x > 14.0f)
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

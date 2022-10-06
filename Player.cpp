#include "Player.h"
#include "./Stage/BlockType.h"

Player::Player() :
	draw(nullptr),
	pos{},
	direction(Player::Direction::UP),
	object(Engine::FUNCTION_ERROR),
	graph(Engine::FUNCTION_ERROR)
{
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
	object = draw->CreateCylinder(0.5f, 0.5f, 3);
	graph = draw->LoadTextrue(L"./Resources/Game/Player.png");
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
	pos = { 0.0f, 0.0f, 0.0f };
}

void Player::Move(const InputManager* const input)
{
	if (input->MainUpTrigger() || input->SubUpTrigger())
	{
		pos.y += 1.0f;
		direction = Player::Direction::UP;
	}
	else if (input->MainLeftTrigger() || input->SubLeftTrigger())
	{
		pos.x -= 1.0f;
		direction = Player::Direction::LEFT;
	}
	else if (input->MainDownTrigger() || input->SubDownTrigger())
	{
		pos.y -= 1.0f;
		direction = Player::Direction::DOWN;
	}
	else if (input->MainRightTrigger() || input->SubRightTrigger())
	{
		pos.x += 1.0f;
		direction = Player::Direction::RIGHT;
	}
}

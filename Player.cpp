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

void Player::Update(const Input* const input)
{
    Move(input);
}

void Player::Draw(const int& offsetX, const int& offsetY)
{
    draw->Draw(
        object,
        pos + Vector3(offsetX, offsetY, 0.0f),
        Math::rotateZ(direction * Math::PI_F / 2),
        Vector3(0.5f, 1.0f, 1.0f),
        DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
        graph
    );
}

void Player::Move(const Input* const input)
{
    if (input->IsKeyTrigger(DIK_W))
    {
        pos.y += 1.0f;
        direction = Player::Direction::UP;
    }
    if (input->IsKeyTrigger(DIK_A))
    {
        pos.x -= 1.0f;
        direction = Player::Direction::LEFT;
    }
    if (input->IsKeyTrigger(DIK_S))
    {
        pos.y -= 1.0f;
        direction = Player::Direction::DOWN;
    }
    if (input->IsKeyTrigger(DIK_D))
    {
        pos.x += 1.0f;
        direction = Player::Direction::RIGHT;
    }
}

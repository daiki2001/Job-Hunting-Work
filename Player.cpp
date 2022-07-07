#include "Player.h"
#include "./Stage/BlockType.h"
#include "./Header/Input.h"
#include "./Header/Error.h"

Player::Player() :
    draw(nullptr),
    posX(0),
    posY(0),
    direction(Player::Direction::UP),
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
    graph = draw->LoadTextrue(L"./Resources/Game/Player.png");
}

void Player::Update()
{
    if (Input::IsKeyTrigger(DIK_W))
    {
        posY--;
        direction = Player::Direction::UP;
    }
    if (Input::IsKeyTrigger(DIK_A))
    {
        posX--;
        direction = Player::Direction::LEFT;
    }
    if (Input::IsKeyTrigger(DIK_S))
    {
        posY++;
        direction = Player::Direction::DOWN;
    }
    if (Input::IsKeyTrigger(DIK_D))
    {
        posX++;
        direction = Player::Direction::RIGHT;
    }
}

void Player::Draw(const int& offsetX, const int& offsetY)
{
    draw->DrawTextrue(
        static_cast<float>((posX * BlockType::WIDTH) + (BlockType::WIDTH / 2) + offsetX),
        static_cast<float>((posY * BlockType::HEIGHT) + (BlockType::HEIGHT / 2) + offsetY),
        static_cast<float>(BlockType::WIDTH),
        static_cast<float>(BlockType::HEIGHT),
        direction * 90.0f,
        graph
    );
}

﻿#include "./Header/DrawPolygon.h"
#include "./Header/Parameter.h"
#include "LoadGraph.h"

const std::string LoadGraph::WALL_BLOCK = "WallBlock";
const std::string LoadGraph::GOAL = "Goal";
const std::string LoadGraph::SWITCH_UI = "SwitchUI";
const std::string LoadGraph::KEY = "Key";
const std::string LoadGraph::BOMB = "Bomb";
const std::string LoadGraph::ARROW = "Arrow";
const std::string LoadGraph::UP_STAIRS = "UpStairs";
const std::string LoadGraph::DOWN_STAIRS = "DownStairs";
const std::string LoadGraph::WHITE_TILE = "WhiteTile";
const std::string LoadGraph::TORCH = "Torch";
const std::string LoadGraph::TRIANGLE = "Triangle";
const std::string LoadGraph::BRACELET = "Bracelet";

const std::string LoadGraph::WALL_UI = "WallUI";
const std::string LoadGraph::KEY_CLOSE = "Key_Close";
const std::string LoadGraph::BREAK_WALL_UI = "BreakWallUI";
const std::string LoadGraph::CREATE_ROOM = "CreateRoom";

const std::string LoadGraph::KEYBOARD = "Keyboard";
const std::string LoadGraph::PUSH_KEYBOARD = "PushKeyboard";
const std::string LoadGraph::CURSOR = "Cursor";

LoadGraph::LoadGraph() :
	isLoad(false)
{
}

LoadGraph::~LoadGraph()
{
}

LoadGraph* LoadGraph::Get()
{
	static LoadGraph instance{};
	return &instance;
}

void LoadGraph::Load()
{
	if (isLoad) return;
	isLoad = true;
	auto draw = Library::DrawPolygon::GetInstance();

	Parameter::Set(WALL_BLOCK.c_str(), draw->LoadTextrue(L"./Resources/Game/Block/WallBlock.png"));
	Parameter::Set(GOAL.c_str(), draw->LoadTextrue(L"./Resources/Game/Block/Goal.png"));
	Parameter::Set(SWITCH_UI.c_str(), draw->LoadTextrue(L"./Resources/UI/SwitchUI.png"));
	Parameter::Set(KEY.c_str(), draw->LoadTextrue(L"./Resources/UI/Key.png"));
	Parameter::Set(BOMB.c_str(), draw->LoadTextrue(L"./Resources/UI/Bomb.png"));
	Parameter::Set(ARROW.c_str(), draw->LoadTextrue(L"./Resources/UI/Arrow.png"));
	Parameter::Set(UP_STAIRS.c_str(), draw->LoadTextrue(L"./Resources/UI/UpStairs.png"));
	Parameter::Set(DOWN_STAIRS.c_str(), draw->LoadTextrue(L"./Resources/UI/DownStairs.png"));
	Parameter::Set(WHITE_TILE.c_str(), draw->LoadTextrue(L"./Resources/Game/WhiteTile.png"));
	Parameter::Set(TORCH.c_str(), draw->LoadTextrue(L"./Resources/UI/Torch.png"));
	Parameter::Set(TRIANGLE.c_str(), draw->LoadTextrue(L"./Resources/UI/TriangleArrow.png"));
	Parameter::Set(BRACELET.c_str(), draw->LoadTextrue(L"./Resources/UI/bracelet.png"));

	Parameter::Set(WALL_UI.c_str(), draw->LoadTextrue(L"./Resources/UI/WallUI.png"));
	Parameter::Set(KEY_CLOSE.c_str(), draw->LoadTextrue(L"./Resources/UI/KeyClose.png"));
	Parameter::Set(BREAK_WALL_UI.c_str(), draw->LoadTextrue(L"./Resources/UI/BreakWallUI.png"));
	Parameter::Set(CREATE_ROOM.c_str(), draw->LoadTextrue(L"./Resources/UI/CreateRoom.png"));

	Parameter::Set(KEYBOARD.c_str(), draw->LoadTextrue(L"./Resources/UI/Keyboard.png"));
	Parameter::Set(PUSH_KEYBOARD.c_str(), draw->LoadTextrue(L"./Resources/UI/PushKeyboard.png"));
	Parameter::Set(CURSOR.c_str(), draw->LoadTextrue(L"./Resources/UI/Cursor.png"));
}

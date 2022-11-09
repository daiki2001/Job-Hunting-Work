#include "LoadGraph.h"
#include "./Header/Parameter.h"

const std::string LoadGraph::WALL_BLOCK = "WallBlock";
const std::string LoadGraph::GOAL = "Goal";
const std::string LoadGraph::SWITCH_UI = "SwitchUI";
const std::string LoadGraph::KEY = "Key";
const std::string LoadGraph::BOMB = "Bomb";

const std::string LoadGraph::WALL_UI = "WallUI";
const std::string LoadGraph::KEY_CLOSE = "Key_Close";
const std::string LoadGraph::BREAK_WALL_UI = "BreakWallUI";
const std::string LoadGraph::CREATE_ROOM = "CreateRoom";

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

void LoadGraph::Load(DrawPolygon* const draw)
{
	if (isLoad) return;
	isLoad = true;

	Parameter::Set(WALL_BLOCK.c_str(), draw->LoadTextrue(L"./Resources/Game/Block/WallBlock.png"));
	Parameter::Set(GOAL.c_str(), draw->LoadTextrue(L"./Resources/Game/Block/Goal.png"));
	Parameter::Set(SWITCH_UI.c_str(), draw->LoadTextrue(L"./Resources/UI/SwitchUI.png"));
	Parameter::Set(KEY.c_str(), draw->LoadTextrue(L"./Resources/UI/Key.png"));
	Parameter::Set(BOMB.c_str(), draw->LoadTextrue(L"./Resources/Game/Block/Bomb.png"));

	Parameter::Set(WALL_UI.c_str(), draw->LoadTextrue(L"./Resources/UI/WallUI.png"));
	Parameter::Set(KEY_CLOSE.c_str(), draw->LoadTextrue(L"./Resources/UI/KeyClose.png"));
	Parameter::Set(BREAK_WALL_UI.c_str(), draw->LoadTextrue(L"./Resources/UI/BreakWallUI.png"));
	Parameter::Set(CREATE_ROOM.c_str(), draw->LoadTextrue(L"./Resources/UI/CreateRoom.png"));
}

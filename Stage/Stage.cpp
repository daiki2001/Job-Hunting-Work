#include "Stage.h"

DrawPolygon* Stage::draw = nullptr;
std::vector<Area> Stage::area;

Stage::Stage()
{
}

Stage::~Stage()
{
}

Stage* Stage::Get()
{
	static Stage instance = {};
	return &instance;
}

void Stage::StaticInit(DrawPolygon* const draw)
{
	Stage::draw = draw;
	Area::StaticInit(Stage::draw);
}

void Stage::Init()
{
}

void Stage::Update()
{
	for (size_t i = 0; i < area.size(); i++)
	{
		area.at(i).Update();
	}
}

void Stage::Draw(const int& offsetX, const int& offsetY)
{
	for (size_t i = 0; i < area.size(); i++)
	{
		area.at(i).Draw(offsetX, offsetY);
	}
}

void Stage::Reset()
{
	for (size_t i = 0; i < area.size(); i++)
	{
		area.at(i).Reset();
	}
}

void Stage::LoadStage(const char* filePath)
{
	area.clear();
	area.emplace_back();
	area.back().LoadArea(filePath);
}

#include "UI.h"
#include "./Header/EngineGeneral.h"
#include "./Header/DrawPolygon.h"
#include "./Input/Input.h"
#include "./Header/Parameter.h"
#include "LoadGraph.h"

int UI::arrow = FUNCTION_ERROR;
int UI::space = FUNCTION_ERROR;

UI* UI::Get()
{
	static UI instance{};
	return &instance;
}

void UI::StaticInit()
{
	auto draw = Library::DrawPolygon::GetInstance();
	arrow = draw->LoadTextrue(L"./Resources/UI/StraightArrow.png");
	space = draw->LoadTextrue(L"./Resources/UI/SpaceKeyboard.png");
}

void UI::WASD(float offsetX, float offsetY, float scale)
{
	const float TEX_SIZE = 64.0f * scale;
	const float FONT_SIZE = 3.0f * scale;
	const float ALPHA = 0.8f;
	int graph = FUNCTION_ERROR;
	const float FONT_POS_Y = 6.0f;
	const float PUSH_FONT_POS_Y = 8.0f;
	DirectX::XMFLOAT2 fontPos = { 20.0f, FONT_POS_Y };
	auto draw = Library::DrawPolygon::GetInstance();

	// W
	if (Input::IsKey(DIK_W))
	{
		graph = Parameter::Get(LoadGraph::PUSH_KEYBOARD.c_str());
		fontPos.y = PUSH_FONT_POS_Y;
	}
	else
	{
		graph = Parameter::Get(LoadGraph::KEYBOARD.c_str());
		fontPos.y = FONT_POS_Y;
	}
	float posX = offsetX + TEX_SIZE;
	float posY = offsetY;
	draw->DrawTextrue(posX, posY, TEX_SIZE, TEX_SIZE, 0.0f, graph, DirectX::XMFLOAT2(0.0f, 0.0f), Color::AddAlphaValue(Color::WHITE, ALPHA));
	draw->DrawString(posX + fontPos.x * scale, posY + fontPos.y * scale, FONT_SIZE, Color::AddAlphaValue(Color::BLACK, ALPHA),
					 Color::AddAlphaValue(Color::BLACK, 0.0f), "W");

	// A
	if (Input::IsKey(DIK_A))
	{
		graph = Parameter::Get(LoadGraph::PUSH_KEYBOARD.c_str());
		fontPos.y = PUSH_FONT_POS_Y;
	}
	else
	{
		graph = Parameter::Get(LoadGraph::KEYBOARD.c_str());
		fontPos.y = FONT_POS_Y;
	}
	posX = offsetX;
	posY = offsetY + TEX_SIZE;
	draw->DrawTextrue(posX, posY, TEX_SIZE, TEX_SIZE, 0.0f, graph, DirectX::XMFLOAT2(0.0f, 0.0f), Color::AddAlphaValue(Color::WHITE, ALPHA));
	draw->DrawString(posX + fontPos.x * scale, posY + fontPos.y * scale, FONT_SIZE, Color::AddAlphaValue(Color::BLACK, ALPHA),
					 Color::AddAlphaValue(Color::BLACK, 0.0f), "A");

	// S
	if (Input::IsKey(DIK_S))
	{
		graph = Parameter::Get(LoadGraph::PUSH_KEYBOARD.c_str());
		fontPos.y = PUSH_FONT_POS_Y;
	}
	else
	{
		graph = Parameter::Get(LoadGraph::KEYBOARD.c_str());
		fontPos.y = FONT_POS_Y;
	}
	posX = offsetX + TEX_SIZE;
	draw->DrawTextrue(posX, posY, TEX_SIZE, TEX_SIZE, 0.0f, graph, DirectX::XMFLOAT2(0.0f, 0.0f), Color::AddAlphaValue(Color::WHITE, ALPHA));
	draw->DrawString(posX + fontPos.x * scale, posY + fontPos.y * scale, FONT_SIZE, Color::AddAlphaValue(Color::BLACK, ALPHA),
					 Color::AddAlphaValue(Color::BLACK, 0.0f), "S");

	// D
	if (Input::IsKey(DIK_D))
	{
		graph = Parameter::Get(LoadGraph::PUSH_KEYBOARD.c_str());
		fontPos.y = PUSH_FONT_POS_Y;
	}
	else
	{
		graph = Parameter::Get(LoadGraph::KEYBOARD.c_str());
		fontPos.y = FONT_POS_Y;
	}
	posX = offsetX + TEX_SIZE * 2.0f;
	draw->DrawTextrue(posX, posY, TEX_SIZE, TEX_SIZE, 0.0f, graph, DirectX::XMFLOAT2(0.0f, 0.0f), Color::AddAlphaValue(Color::WHITE, ALPHA));
	draw->DrawString(posX + fontPos.x * scale, posY + fontPos.y * scale, FONT_SIZE, Color::AddAlphaValue(Color::BLACK, ALPHA),
					 Color::AddAlphaValue(Color::BLACK, 0.0f), "D");
}

void UI::Arrow(float offsetX, float offsetY, float scale)
{
	const float TEX_SIZE = 64.0f * scale;
	const float ARROW_SIZE = 48.0f * scale;
	const float ALPHA = 0.8f;
	int graph = FUNCTION_ERROR;
	const float ARROW_POS_Y = -1.0f;
	const float PUSH_ARROW_POS_Y = ARROW_POS_Y + 2.0f;
	DirectX::XMFLOAT2 arrowPos = { 0.0f, ARROW_POS_Y };

	float posX = offsetX + TEX_SIZE;
	float posY = offsetY;
	UpDownArrow(posX, posY, scale);

	posX = offsetX;
	posY = offsetY + TEX_SIZE;
	LeftRightArrow(posX, posY, scale);
}

void UI::UpDownArrow(float offsetX, float offsetY, float scale)
{
	const float TEX_SIZE = 64.0f * scale;
	const float ARROW_SIZE = 48.0f * scale;
	const float ALPHA = 0.8f;
	int graph = FUNCTION_ERROR;
	const float ARROW_POS_Y = -2.0f;
	const float PUSH_ARROW_POS_Y = ARROW_POS_Y + 2.0f;
	DirectX::XMFLOAT2 arrowPos = { 0.0f, ARROW_POS_Y };
	auto draw = Library::DrawPolygon::GetInstance();

	// ª
	if (Input::IsKey(DIK_UP))
	{
		graph = Parameter::Get(LoadGraph::PUSH_KEYBOARD.c_str());
		arrowPos.y = PUSH_ARROW_POS_Y;
	}
	else
	{
		graph = Parameter::Get(LoadGraph::KEYBOARD.c_str());
		arrowPos.y = ARROW_POS_Y;
	}
	float posX = offsetX;
	float posY = offsetY;
	draw->DrawTextrue(posX, posY, TEX_SIZE, TEX_SIZE, 0.0f, graph, DirectX::XMFLOAT2(0.0f, 0.0f), Color::AddAlphaValue(Color::WHITE, ALPHA));
	draw->DrawTextrue(posX + arrowPos.x + TEX_SIZE /2.0f, posY + arrowPos.y + TEX_SIZE / 2.0f, ARROW_SIZE, ARROW_SIZE, Math::PI_F, arrow,
					  DirectX::XMFLOAT2(0.5f, 0.5f), Color::AddAlphaValue(Color::WHITE, ALPHA));

	// «
	if (Input::IsKey(DIK_DOWN))
	{
		graph = Parameter::Get(LoadGraph::PUSH_KEYBOARD.c_str());
		arrowPos.y = PUSH_ARROW_POS_Y;
	}
	else
	{
		graph = Parameter::Get(LoadGraph::KEYBOARD.c_str());
		arrowPos.y = ARROW_POS_Y;
	}
	posY = offsetY + TEX_SIZE;
	draw->DrawTextrue(posX, posY, TEX_SIZE, TEX_SIZE, 0.0f, graph, DirectX::XMFLOAT2(0.0f, 0.0f), Color::AddAlphaValue(Color::WHITE, ALPHA));
	draw->DrawTextrue(posX + arrowPos.x + TEX_SIZE / 2.0f, posY + arrowPos.y + TEX_SIZE / 2.0f, ARROW_SIZE, ARROW_SIZE, 0.0f, arrow,
					  DirectX::XMFLOAT2(0.5f, 0.5f), Color::AddAlphaValue(Color::WHITE, ALPHA));
}

void UI::LeftRightArrow(float offsetX, float offsetY, float scale)
{
	const float TEX_SIZE = 64.0f * scale;
	const float ARROW_SIZE = 48.0f * scale;
	const float ALPHA = 0.8f;
	int graph = FUNCTION_ERROR;
	const float ARROW_POS_Y = -2.0f;
	const float PUSH_ARROW_POS_Y = ARROW_POS_Y + 2.0f;
	DirectX::XMFLOAT2 arrowPos = { 0.0f, ARROW_POS_Y };
	auto draw = Library::DrawPolygon::GetInstance();

	// ©
	if (Input::IsKey(DIK_LEFT))
	{
		graph = Parameter::Get(LoadGraph::PUSH_KEYBOARD.c_str());
		arrowPos.y = PUSH_ARROW_POS_Y;
	}
	else
	{
		graph = Parameter::Get(LoadGraph::KEYBOARD.c_str());
		arrowPos.y = ARROW_POS_Y;
	}
	float posX = offsetX;
	float posY = offsetY;
	draw->DrawTextrue(posX, posY, TEX_SIZE, TEX_SIZE, 0.0f, graph, DirectX::XMFLOAT2(0.0f, 0.0f), Color::AddAlphaValue(Color::WHITE, ALPHA));
	draw->DrawTextrue(posX + arrowPos.x + TEX_SIZE / 2.0f, posY + arrowPos.y + TEX_SIZE / 2.0f, ARROW_SIZE, ARROW_SIZE, Math::PI_F / 2.0f, arrow,
					  DirectX::XMFLOAT2(0.5f, 0.5f), Color::AddAlphaValue(Color::WHITE, ALPHA));

	// ¨
	if (Input::IsKey(DIK_RIGHT))
	{
		graph = Parameter::Get(LoadGraph::PUSH_KEYBOARD.c_str());
		arrowPos.y = PUSH_ARROW_POS_Y;
	}
	else
	{
		graph = Parameter::Get(LoadGraph::KEYBOARD.c_str());
		arrowPos.y = ARROW_POS_Y;
	}
	posX = offsetX + TEX_SIZE * 2.0f;
	draw->DrawTextrue(posX, posY, TEX_SIZE, TEX_SIZE, 0.0f, graph, DirectX::XMFLOAT2(0.0f, 0.0f), Color::AddAlphaValue(Color::WHITE, ALPHA));
	draw->DrawTextrue(posX + arrowPos.x + TEX_SIZE / 2.0f, posY + arrowPos.y + TEX_SIZE / 2.0f, ARROW_SIZE, ARROW_SIZE, Math::PI_F * 3.0f / 2.0f, arrow,
					  DirectX::XMFLOAT2(0.5f, 0.5f), Color::AddAlphaValue(Color::WHITE, ALPHA));
}

void UI::Space(float offsetX, float offsetY, float scale)
{
	const DirectX::XMFLOAT2 TEX_SIZE = { 128.0f, 64.0f };
	const DirectX::XMFLOAT2 DRAW_SIZE = { 128.0f * scale, 64.0f * scale };
	const float ALPHA = 0.8f;
	DirectX::XMFLOAT2 texPos = { 0.0f, 0.0f };

	if (Input::IsKey(DIK_SPACE))
	{
		texPos.y = TEX_SIZE.y;
	}
	else
	{
		texPos.y = 0.0f;
	}
	Library::DrawPolygon::GetInstance()->DrawCutTextrue(offsetX, offsetY, DRAW_SIZE.x, DRAW_SIZE.y, texPos, TEX_SIZE, 0.0f, space,
						 DirectX::XMFLOAT2(0.0f, 0.0f), Color::AddAlphaValue(Color::WHITE, ALPHA));
}

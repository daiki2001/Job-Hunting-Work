#include "Item.h"
#include "./Input/GameInput.h"
#include "./Header/Parameter.h"
#include "./Header/Error.h"

DrawPolygon* Item::draw = nullptr;

Item::Item(unsigned int maxCount, unsigned int count) :
	graph(FUNCTION_ERROR),
	count(count),
	maxCount(maxCount)
{
}

void Item::Init(const wchar_t* fileName)
{
	if (fileName == nullptr) return;

	if (graph == FUNCTION_ERROR)
	{
		graph = draw->LoadTextrue(fileName);
	}
}

void Item::Init(int texNumber)
{
	graph = texNumber;
}

void Item::StaticInit(DrawPolygon* const draw)
{
	Item::draw = draw;
}

void Item::DrawInfo(const char* type, int offsetX, int offsetY, float scale)
{
	DirectDrawing::ChangeSpriteShader();

	if (graph == FUNCTION_ERROR)
	{
		draw->DrawString(static_cast<float>(offsetX), static_cast<float>(offsetY), 2.0f * scale, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						 "%sCount:%d", type, count);
	}
	else
	{
		draw->DrawTextrue(static_cast<float>(offsetX), static_cast<float>(offsetY), 32.0f * scale, 32.0f * scale, 0.0f, Parameter::Get("white1x1"), DirectX::XMFLOAT2(0.0f, 0.0f));
		draw->DrawTextrue(static_cast<float>(offsetX), static_cast<float>(offsetY), 32.0f * scale, 32.0f * scale, 0.0f, graph, DirectX::XMFLOAT2(0.0f, 0.0f));
		draw->DrawString(static_cast<float>(offsetX) + 32.0f * scale, static_cast<float>(offsetY), 2.0f * scale, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						 ":%d", count);
	}
}

bool Item::Acquisition()
{
	if (GameInput::Get()->DecisionTrigger() == false) return false;

	if (count < maxCount)
	{
		count++;
		return true;
	}

	return false;
}

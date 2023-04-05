#include "Item.h"
#include "./Input/GameInput.h"
#include "./Effect/BombParticle.h"
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
	Particle::StaticInit(draw);
	BombParticle::Init();
}

void Item::DrawInfo(const char* type, int offsetX, int offsetY, float scale)
{
	DirectDrawing::ChangeSpriteShader();

	if (graph == FUNCTION_ERROR)
	{
		draw->DrawString(static_cast<float>(offsetX), static_cast<float>(offsetY), 2.0f * scale,
						 Color::AddAlphaValue(Color::WHITE, 1.0f), Color::AddAlphaValue(Color::BLACK, 0.5f),
						 "%sCount:%d", type, count);
	}
	else
	{
		float size = 32.0f * scale;
		draw->DrawString(static_cast<float>(offsetX) + size * 0.625f, static_cast<float>(offsetY), 2.0f * scale,
						 Color::AddAlphaValue(Color::WHITE, 1.0f), Color::AddAlphaValue(Color::BLACK, 0.5f),
						 " %d ", count);
		draw->DrawTextrue(static_cast<float>(offsetX), static_cast<float>(offsetY), size, size, 0.0f,
						  Parameter::Get("white1x1"), DirectX::XMFLOAT2(0.0f, 0.0f));
		draw->DrawTextrue(static_cast<float>(offsetX), static_cast<float>(offsetY), size, size, 0.0f,
						  graph, DirectX::XMFLOAT2(0.0f, 0.0f));
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

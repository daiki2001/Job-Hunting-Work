#include "Item.h"

DrawPolygon* Item::draw = nullptr;

Item::Item(unsigned int maxCount, unsigned int count) :
	count(count),
	maxCount(maxCount)
{
}

void Item::Draw(const char* type, int offsetX, int offsetY)
{
	DirectDrawing::ChangeSpriteShader();

	draw->DrawString(static_cast<float>(offsetX), static_cast<float>(offsetY), 2.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
					 "%sCount:%d", type, count);
}

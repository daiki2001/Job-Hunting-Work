#include "Scroll.h"
#include "./Header/EngineGeneral.h"

Scroll::Scroll() :
	isScroll(false),
	time(0.0f)
{
}

void Scroll::ScrollStart()
{
	isScroll = true;
	time = 0.0f;
}

void Scroll::ScrollUpdate(float addTime)
{
	if (isScroll == false) { return; }

	time = Clamp(time + addTime, 1.0f, 0.0f);

	if (time >= 1.0f) { isScroll = false; }
}

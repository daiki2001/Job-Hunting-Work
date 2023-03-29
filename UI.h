#pragma once
#include "./Header/DrawPolygon.h"

class UI final
{
public: //シングルトン化
	static UI* Get();
private:
	UI() {}
	UI(const UI&) = delete;
	UI operator=(const UI&) = delete;
	~UI() {}

private: //メンバ静的変数
	static DrawPolygon* draw;
	static int arrow;
	static int space;

public: //メンバ静的関数
	static void StaticInit(DrawPolygon* draw);

public: //メンバ関数
	void WASD(float offsetX = 0.0f, float offsetY = 0.0f, float scale = 1.0f);
	void Arrow(float offsetX = 0.0f, float offsetY = 0.0f, float scale = 1.0f);
	void UpDownArrow(float offsetX = 0.0f, float offsetY = 0.0f, float scale = 1.0f);
	void LeftRightArrow(float offsetX = 0.0f, float offsetY = 0.0f, float scale = 1.0f);
	void Space(float offsetX = 0.0f, float offsetY = 0.0f, float scale = 1.0f);
};
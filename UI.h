#pragma once
#include "./Header/DrawPolygon.h"

class UI final
{
public: //�V���O���g����
	static UI* Get();
private:
	UI() {}
	UI(const UI&) = delete;
	UI operator=(const UI&) = delete;
	~UI() {}

private: //�����o�ÓI�ϐ�
	static DrawPolygon* draw;
	static int arrow;
	static int space;

public: //�����o�ÓI�֐�
	static void StaticInit(DrawPolygon* draw);

public: //�����o�֐�
	void WASD(float offsetX = 0.0f, float offsetY = 0.0f, float scale = 1.0f);
	void Arrow(float offsetX = 0.0f, float offsetY = 0.0f, float scale = 1.0f);
	void UpDownArrow(float offsetX = 0.0f, float offsetY = 0.0f, float scale = 1.0f);
	void LeftRightArrow(float offsetX = 0.0f, float offsetY = 0.0f, float scale = 1.0f);
	void Space(float offsetX = 0.0f, float offsetY = 0.0f, float scale = 1.0f);
};
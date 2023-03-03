#pragma once

class FrameDraw
{
private: //メンバ関数


public: //メンバ関数
	FrameDraw();
	~FrameDraw();

	void Draw(int sizeX, int sizeY, int offsetX = 0, int offsetY = 0, float scale = 1.0f);
};

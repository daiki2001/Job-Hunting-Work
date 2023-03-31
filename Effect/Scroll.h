#pragma once

class Scroll
{
private: //メンバ変数
	bool isScroll;
	float time;

public: //メンバ関数
	Scroll();
	~Scroll() = default;

	// スクロールの開始
	void ScrollStart();
	// スクロールの更新
	void ScrollUpdate(float addTime);

	const bool GetFlag() const { return isScroll; }
	const float GetTime() const { return time; }
};

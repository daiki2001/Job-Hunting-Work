#pragma once
#include "./Header/DrawPolygon.h"

class Item
{
public: //静的メンバ変数
	static DrawPolygon* draw;

public: //静的メンバ関数
	static void StaticInit(DrawPolygon* const draw) { Item::draw = draw; }

private: //メンバ変数
	unsigned int count;    //所持数
	unsigned int maxCount; //アイテムの上限

public: //メンバ関数
	Item(unsigned int maxCount, unsigned int count = 0);
	~Item() = default;

	// 描画
	void Draw(const char* type, int offsetX = 0, int offsetY = 0);
	// リセット
	void Reset() { count = 0; }

	// アイテムの入手
	void Acquisition() { if (count < maxCount) count++; }
	// アイテムの使用
	void Use() { if (count > 0) count--; }

	// 所持数の取得
	const unsigned int GetCount() const { return count; }

};
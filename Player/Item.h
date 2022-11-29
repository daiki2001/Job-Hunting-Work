#pragma once
#include "./Header/DrawPolygon.h"

class Item
{
protected: //静的メンバ変数
	static DrawPolygon* draw;

public: //静的メンバ関数
	static void StaticInit(DrawPolygon* const draw);

public: //メンバ変数
	int graph; //画像情報
private:
	unsigned int count;    //所持数
	unsigned int maxCount; //アイテムの上限

public: //メンバ関数
	Item(unsigned int maxCount, unsigned int count = 0);
	virtual ~Item() = default;

	// 初期化処理
	void Init(const wchar_t* fileName = nullptr);
	void Init(int texNumber);
	// 描画処理
	virtual void Draw(int offsetX = 0, int offsetY = 0) {}
	// リセット
	void Reset() { count = 0; }
	// 所持数の表示
	void DrawInfo(const char* type, int offsetX = 0, int offsetY = 0, float scale = 1.0f);

	// アイテムの入手
	void Acquisition() { if (count < maxCount) count++; }
	// アイテムの使用
	void Use() { if (count > 0) count--; }

	// 所持数の取得
	const unsigned int GetCount() const { return count; }

};
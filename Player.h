#pragma once
#include "./Header/DrawPolygon.h"

class Player final
{
public: //シングルトン化
	static Player* Get();
private:
	Player();
	Player(const Player&) = delete;
	~Player();
	Player operator=(const Player&) = delete;

public: //サブクラス
	enum Direction
	{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

private: //メンバ変数
	DrawPolygon* draw;

	int posX, posY;      //座標
	Direction direction; //向いている方向
	int graph;           //画像

public: //メンバ関数
	// 初期化処理
	void Init(DrawPolygon* const draw);
	// 更新処理
	void Update();
	// 描画処理
	void Draw(const int& offsetX = 0, const int& offsetY = 0);
};

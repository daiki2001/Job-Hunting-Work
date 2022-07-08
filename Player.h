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
		LEFT = -1,
		UP,
		RIGHT,
		DOWN
	};

public: //メンバ変数
	int posX, posY; //座標
private:
	DrawPolygon* draw;

	Direction direction; //向いている方向
	int graph;           //画像

public: //メンバ関数
	// 初期化処理
	void Init(DrawPolygon* const draw);
	// 更新処理
	void Update();
	// 描画処理
	void Draw(const int& offsetX = 0, const int& offsetY = 0);

	const Direction GetDirection() const { return direction; };
};

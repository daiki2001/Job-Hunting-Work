#pragma once
#include "./Header/DrawPolygon.h"
#include "./Header/Input.h"
#include "./Header/EngineGeneral.h"

class Player final
{
public: //シングルトン化
	static Player* Get();
private:
	Player();
	Player(const Player&) = delete;
	~Player();
	Player operator=(const Player&) = delete;

private: //エイリアス
	using Vector3 = Math::Vector3;

public: //サブクラス
	enum Direction
	{
		RIGHT = -1,
		UP,
		LEFT,
		DOWN
	};

public: //メンバ変数
	Vector3 pos; //座標
private:
	DrawPolygon* draw;

	Direction direction; //向いている方向
	int object;          //プレイヤーのオブジェクト
	int graph;           //画像

public: //メンバ関数
	// 初期化処理
	void Init(DrawPolygon* const draw);
	// 更新処理
	void Update(const Input* const input);
	// 描画処理
	void Draw(const int& offsetX = 0, const int& offsetY = 0);

	void Reset();

	const Direction GetDirection() const { return direction; };
private:
	void Move(const Input* const input);
};

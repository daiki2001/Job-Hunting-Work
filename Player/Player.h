#pragma once
#include "./Header/DrawPolygon.h"
#include "../InputManager.h"
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
	
public: //定数
	static const float SPEED;

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

	/*インベントリ*/
	unsigned int keyCount; //所持している鍵の数

public: //メンバ関数
	// 初期化処理
	void Init(DrawPolygon* const draw);
	// 更新処理
	void Update(const InputManager* const input);
	// 描画処理
	void Draw(int offsetX = 0, int offsetY = 0);
	// リセット
	void Reset();

	// 鍵の獲得
	void AcquisitionKey() { keyCount++; }
	// 鍵の使用
	void UseKey() { if(keyCount > 0) keyCount--; }

	const Direction GetDirection() const { return direction; }
	const unsigned int GetKeyCount() const { return keyCount; }
private:
	void Move(const InputManager* const input);
};

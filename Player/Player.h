#pragma once
#include "./Header/DrawPolygon.h"
#include "Item.h"
#include "Bomb.h"
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
	static const Vector3 COLLISION_SIZE;

public: //サブクラス
	enum Direction
	{
		RIGHT = -1,
		UP,
		LEFT,
		DOWN
	};
	enum SelectItem
	{
		KEY,  //鍵
		BOMB, //爆弾
		MAX   //上限(プログラム用)
	};

public: //メンバ変数
	Vector3 pos; //座標
private:
	DrawPolygon* draw;

	Direction direction; //向いている方向
	int object;          //プレイヤーのオブジェクト

	/*インベントリ*/
	SelectItem selectItem;
	Item key;  //鍵
	Bomb bomb; //爆弾

public: //メンバ関数
	// 初期化処理
	void Init(DrawPolygon* const draw);
	// 更新処理
	void Update(const InputManager* const input);
	// 描画処理
	void Draw(int offsetX = 0, int offsetY = 0);
	// 描画処理(アイテムインベントリ)
	void DrawInventory(int offsetX = 0, int offsetY = 0, float scale = 1.0f);
	// リセット
	void Reset();

	// 鍵の獲得
	void AcquisitionKey() { key.Acquisition(); }
	// 爆弾の獲得
	void AcquisitionBomb() { bomb.Acquisition(); }
	// 鍵の使用
	void UseKey() { key.Use(); }
	// 爆弾の使用
	void UseBomb() { bomb.Use(); }

	const Direction GetDirection() const { return direction; }
	const unsigned int GetKeyCount() const { return key.GetCount(); }
	const unsigned int GetBombCount() const { return bomb.GetCount(); }
private:
	void Move(const InputManager* const input);
	void SelectAction(const InputManager* const input);
	void Action(const InputManager* const input);
	void MoveUp(const InputManager* const input);
	void MoveDown(const InputManager* const input);
	void MoveLeft(const InputManager* const input);
	void MoveRight(const InputManager* const input);
	void KeyAction();
	void BombAction();
};

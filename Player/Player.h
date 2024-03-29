﻿#pragma once
#include "Item.h"
#include "Bomb.h"
#include "./Header/EngineGeneral.h"
#include <vector>

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
		TOP,
		LEFT,
		BOTTOM,
		COUNT = 4
	};
	enum SelectItem
	{
		BRACELET, //ブレスレット
		KEY,      //鍵
		BOMB,     //爆弾
		MAX       //種類数(プログラム用)
	};

public: //メンバ変数
	Vector3 pos; //座標
private:
	Direction direction; //向いている方向
	int object;          //プレイヤーのオブジェクト
	Vector3 animationPos; //自機の部屋移動中のアニメーション時の座標

	/*インベントリ*/
	SelectItem selectItem;
	Item key;  //鍵
	Bomb bomb; //爆弾
	bool isBracelet;

	std::vector<int> route; //迷いの森を進んだ道

public: //メンバ関数
	// 初期化処理
	void Init();
	// 更新処理
	void Update();
	// 描画処理
	void Draw(int offsetX = 0, int offsetY = 0);
	// 描画処理(アイテムインベントリ)
	void DrawInventory(int offsetX = 0, int offsetY = 0, float scale = 1.0f);
	// リセット
	void Reset();

	// 自機の部屋移動中のアニメーション
	void MovingRoom();

	// 鍵の獲得
	bool AcquisitionKey() { return key.Acquisition(); }
	// 爆弾の獲得
	bool AcquisitionBomb() { return bomb.Acquisition(); }
	// 鍵の使用
	void UseKey() { key.Use(); }
	// 爆弾の使用
	void UseBomb() { bomb.Use(); }
	// 鍵の獲得
	bool AcquisitionBracelet();

	const Direction GetDirection() const { return direction; }
	const SelectItem GetAction() { return selectItem; }
	const unsigned int GetKeyCount() const { return key.GetCount(); }
	const unsigned int GetBombCount() const { return bomb.GetCount(); }
private:
	void Move();
	void SelectAction();
	void Action();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void KeyAction();
	void BombAction();
};

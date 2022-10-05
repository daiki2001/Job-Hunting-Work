#pragma once
#include "./Header/DrawPolygon.h"
#include "BlockManager.h"
#include "Door.h"
#include <stdio.h>
#include "./Header/EngineGeneral.h"

class Area
{
private: //エイリアス
	using Vector3 = Math::Vector3;

public: //サブクラス
	enum DoorNum
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

private: //静的メンバ変数
	static DrawPolygon* draw;
	static int wall_obj; //外壁のオブジェクト

public: //静的メンバ関数
	// 静的初期化処理
	static void StaticInit(DrawPolygon* const draw);

private: //メンバ変数
	BlockManager block_mgr;
	Door door[4];

public: //メンバ関数
	Area();
	~Area();

	// 初期化処理
	void Init() {}
	// 更新処理
	void Update();
	// 描画処理
	void Draw(const int& offsetX = 0, const int& offsetY = 0);
	// リセット処理
	void Reset();

	// ステージ読み込み
	int LoadArea(FILE* fileHandle);

	const bool IsGoal() { return block_mgr.GetGoal(); }

	BlockManager* GetBlockManager() { return &block_mgr; }
	Door::DoorStatus GetDoorStatus(const DoorNum& num) { return door[num].GetStatus(); }
};

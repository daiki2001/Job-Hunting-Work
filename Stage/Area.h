#pragma once
#include "./Header/DrawPolygon.h"
#include "BlockManager.h"
#include "./Header/EngineGeneral.h"

class Area
{
private: //エイリアス
	using Vector3 = Math::Vector3;

private: //静的メンバ変数
	static DrawPolygon* draw;
	static BlockManager* block_mgr;
	static int wall_obj; //外壁のオブジェクト
	static int door_obj; //ドアのオブジェクト

public: //静的メンバ関数
	static const bool IsGoal() { return block_mgr->GetGoal(); }

	static BlockManager* GetBlockManager() { return block_mgr; }

public: //メンバ関数
	Area();
	~Area();

	// 初期化処理
	void Init(DrawPolygon* const draw);
	// 更新処理
	void Update();
	// 描画処理
	void Draw(const int& offsetX = 0, const int& offsetY = 0);
	// リセット処理
	void Reset();

	// ステージ読み込み
	void LoadArea(const char* filePath = nullptr);
};

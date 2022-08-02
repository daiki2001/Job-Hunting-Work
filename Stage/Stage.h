#pragma once
#include "./Header/DrawPolygon.h"
#include "BlockManager.h"
#include "./Header/EngineGeneral.h"

class Stage final
{
public: //シングルトン化
	static Stage* Get();
private:
	Stage();
	Stage(const Stage&) = delete;
	~Stage();
	Stage operator=(const Stage&) = delete;

private: //エイリアス
	using Vector3 = Math::Vector3;

private: //静的メンバ変数
	static DrawPolygon* draw;
	static BlockManager* block_mgr;
	static int wall_obj; //外壁のオブジェクト
	static int door_obj; //ドアのオブジェクト

public: //静的メンバ関数
	static const bool IsGoal() { return block_mgr->GetGoal(); };

public: //メンバ関数
	// 初期化処理
	void Init(DrawPolygon* const draw);
	// 更新処理
	void Update();
	// 描画処理
	void Draw(const int& offsetX = 0, const int& offsetY = 0);
	// ステージ読み込み
	void LoadStage(const char* filePath = nullptr);
};

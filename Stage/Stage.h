#pragma once
#include "./Header/DrawPolygon.h"
#include "BlockManager.h"

class Stage final
{
public: //シングルトン化
	static Stage* Get();
private:
	Stage();
	Stage(const Stage&) = delete;
	~Stage();
	Stage operator=(const Stage&) = delete;

private: //静的メンバ変数
	static BlockManager* blockMgr;

private: //メンバ変数
	//DrawPolygon* const draw;

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

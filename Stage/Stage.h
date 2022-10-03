#pragma once
#include "./Header/DrawPolygon.h"
#include "Area.h"
#include "./Header/EngineGeneral.h"
#include <vector>

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
	static std::vector<Area> area; //各部屋の情報

public: //静的メンバ関数
	// 静的初期化処理
	static void StaticInit(DrawPolygon* const draw);

	static const bool IsGoal() { return Area::IsGoal(); }

public: //メンバ関数
	// 初期化処理
	void Init();
	// 更新処理
	void Update();
	// 描画処理
	void Draw(const int& offsetX = 0, const int& offsetY = 0);
	// リセット処理
	void Reset();

	// ステージ読み込み
	void LoadStage(const char* filePath = nullptr);
};

#pragma once
#include "./Header/DrawPolygon.h"
#include "BlockManager.h"
#include "Door.h"
#include "./Header/EngineGeneral.h"
#include <vector>
#include <stdio.h>

class Area
{
private: //エイリアス
	using Vector3 = Math::Vector3;

public: //定数
	static const size_t STAGE_WIDTH;
	static const size_t STAGE_HEIGHT;
	static const float WALL_SIZE;
	static const Vector3 INIT_CAMERA;
	static const int NONE_LOST_FOREST;
	static const size_t MAX_COURSE_NUM;

public: //サブクラス
	enum DoorNum
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		MAX
	};

private: //静的メンバ変数
	static DrawPolygon* draw;
	static int wall_obj; //外壁のオブジェクト
	static Area* planeArea; //ブロックが何も置かれていないエリア

public: //静的メンバ関数
	// 静的初期化処理
	static void StaticInit(DrawPolygon* const draw);

	static const int GetWallObj() { return wall_obj; }
	static const Area GetPlaneArea() { return *planeArea; }
private:
	static void DrawWall(const Vector3& offset);

public: //メンバ変数
	bool isAlive;
private:
	BlockManager block_mgr;
	Door door[4];
	Door doorInit[4];
	std::vector<int> lostForest; //迷いの森

public: //メンバ関数
	Area();
	~Area();

	// 初期化処理
	void Init() {}
	// 更新処理
	void Update();
	// 描画処理
	void Draw(const Vector3& offset = Vector3::Zero());
	// リセット処理
	void Reset();

	// ステージ読み込み
	int LoadArea(FILE* fileHandle);
	// ステージ書き込み
	int WriteArea(FILE* fileHandle);

	const bool IsGoal() { return block_mgr.GetGoal(); }
	const int LostForest(const std::vector<int>& route, const size_t& index = 0);

	BlockManager* GetBlockManager() { return &block_mgr; }
	Door::DoorStatus GetDoorStatus(DoorNum num) { return door[num].GetStatus(); }
	Door& GetDoor(DoorNum num) { return door[num]; }
	const std::vector<int>& GetRoute() { return lostForest; }
	int GetRoute(const size_t& index) { return (index < lostForest.size()) ? lostForest[index] : FUNCTION_ERROR; }

	void SetDoorInit(Door doors[4]);
	void SetDoorInit(Door::DoorStatus up, Door::DoorStatus down,
					 Door::DoorStatus left, Door::DoorStatus right);
	void SetDoorStatus(Door::DoorStatus status, DoorNum num) { door[num].SetStatus(status); }
	void SetRoute(const std::vector<int>& route) { lostForest = route; }
	int SetRoute(const size_t& index, int route);
};

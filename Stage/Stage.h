#pragma once
#include "./Header/DrawPolygon.h"
#include "Area.h"
#include <vector>
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

public: //サブクラス
	struct Room
	{
		Area area = {}; //部屋の情報
		int connection[4] = { -1, -1, -1, -1 }; //どの部屋と繋がっているか
	};

private: //静的メンバ変数
	static DrawPolygon* draw;
	static Player* player;         //プレイヤー
	static std::vector<Room> room; //各部屋の情報
	static int nowRoom; //プレイヤーが現在いる部屋

public: //静的メンバ関数
	// 静的初期化処理
	static void StaticInit(DrawPolygon* const draw);

	static int MoveUpRoom();
	static int MoveDownRoom();
	static int MoveLeftRoom();
	static int MoveRightRoom();

	static const bool IsGoal();

	static BlockManager* GetBlockManager()
	{
		if (room.size() == 0) return nullptr;
		return room[nowRoom].area.GetBlockManager();
	}
	static Door::DoorStatus GetDoorStatus(Area::DoorNum num)
	{
		if (room.size() == 0) return Door::DoorStatus::OPEN;
		return room[nowRoom].area.GetDoorStatus(num);
	}

	static void SetDoorStatus(Door::DoorStatus status, Area::DoorNum num)
	{
		if (room.size() == 0) return;
		room[nowRoom].area.SetDoorStatus(status, num);
	}

public: //メンバ関数
	// 初期化処理
	void Init();
	// 更新処理
	void Update();
	// 描画処理
	void Draw(int offsetX = 0, int offsetY = 0);
	// リセット処理
	void Reset();

	// ステージ読み込み
	int LoadStage(const char* filePath = nullptr);
};

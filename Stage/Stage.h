#pragma once
#include "./Header/DrawPolygon.h"
#include "Area.h"
#include <map>
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
	static std::map<Vector3, Area> rooms; //各部屋の情報
	static Vector3 nowRoom; //プレイヤーが現在いる部屋

public: //静的メンバ関数
	// 静的初期化処理
	static void StaticInit(DrawPolygon* const draw);

	static int CreateRoom(int direction = -1);
	static int MoveUpRoom();
	static int MoveDownRoom();
	static int MoveLeftRoom();
	static int MoveRightRoom();
	static void AllDeleteRoom();

	static const bool IsGoal();

	static BlockManager* GetBlockManager()
	{
		if (rooms.empty()) return nullptr;
		return rooms[nowRoom].GetBlockManager();
	}
	static Door::DoorStatus GetDoorStatus(Area::DoorNum num)
	{
		if (rooms.empty()) return Door::DoorStatus::OPEN;
		return rooms[nowRoom].GetDoorStatus(num);
	}

	static void SetDoorStatus(Door::DoorStatus status, Area::DoorNum num)
	{
		if (rooms.empty()) return;
		rooms[nowRoom].SetDoorStatus(status, num);
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

	// 最後の部屋に飛ぶ
	void LastRoom();

	// ステージ読み込み
	int LoadStage(const char* filePath = nullptr);
	// ステージ書き込み
	int WirteStage(const char* filePath = nullptr);

	Area& GetArea()
	{
		static Area defArea = {};
		if (rooms.empty()) return defArea;
		return rooms[nowRoom];
	}
};

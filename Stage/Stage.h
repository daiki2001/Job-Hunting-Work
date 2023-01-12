#pragma once
#include "./Header/DrawPolygon.h"
#include "Area.h"
#include "./Effect/Scroll.h"
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

private: //定数
	static const Vector3 moveUpRoom;
	static const Vector3 moveDownRoom;
	static const Vector3 moveLeftRoom;
	static const Vector3 moveRightRoom;

public: //サブクラス
	struct Room
	{
		Area area = {}; //部屋の情報
		int connection[4] = { -1, -1, -1, -1 }; //どの部屋と繋がっているか
	};

public: //静的メンバ変数
	static Scroll scroll;
private:
	static DrawPolygon* draw;
	static Player* player;         //プレイヤー
	static std::map<Vector3, Area> rooms; //各部屋の情報
	static Vector3 nowRoom; //プレイヤーが現在いる部屋
	static Vector3 oldRoom; //プレイヤーが1つ前にいた部屋
	
public: //静的メンバ関数
	// 静的初期化処理
	static void StaticInit(DrawPolygon* const draw);

	// 部屋の追加
	static int CreateRoom(int direction = -1);
	// 今いる部屋の削除
	static int DeleteRoom(int direction);
	// 部屋の削除
	static int DeleteRoom(int direction, const Vector3& roomPos);
	// 全部屋の削除
	static void AllDeleteRoom();

	static int MoveUpRoom();
	static int MoveDownRoom();
	static int MoveLeftRoom();
	static int MoveRightRoom();

	static const bool IsGoal();

	static Vector3 GetRoom() { return nowRoom; }
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

	static int SetRoom(const Vector3& roomPos)
	{
		if (rooms.find(roomPos) == rooms.end())
		{
			return FUNCTION_ERROR;
		}

		nowRoom = roomPos;
		return 0;
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

	// ステージのスクロール描画
	void ScrollDraw(int offsetX = 0, int offsetY = 0);

	// ステージ読み込み
	int LoadStage(const char* filePath = nullptr);
	// ステージ書き込み
	int WirteStage(const char* filePath = nullptr);

	// 最後の部屋に飛ぶ
	void LastRoom();

	Area& GetArea()
	{
		static Area defArea = Area::GetPlaneArea();
		if (rooms.empty()) return defArea;
		return rooms[nowRoom];
	}
	Area& GetArea(const Vector3& roomPos)
	{
		static Area defArea = Area::GetPlaneArea();
		if (rooms.find(roomPos) == rooms.end()) return defArea;
		return rooms[roomPos];
	}
private:
	// ミニマップの描画
	void MiniMap(int offsetX = 0, int offsetY = 0, float scale = 1.0f, DirectX::XMFLOAT2 scroll = { 0.0f, 0.0f });
};

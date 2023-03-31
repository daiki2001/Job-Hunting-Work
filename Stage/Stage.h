#pragma once
#include "./Header/DrawPolygon.h"
#include "Area.h"
#include "../Effect/Scroll.h"
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

public: //定数
	static const Vector3 LEFT_ROOM;  //左の部屋
	static const Vector3 RIGHT_ROOM; //右の部屋
	static const Vector3 FRONT_ROOM; //手前の部屋
	static const Vector3 BACK_ROOM;  //奥の部屋
	static const Vector3 UP_FLOOR;   //上の階
	static const Vector3 DOWN_FLOOR; //下の階

public: //サブクラス

public: //静的メンバ変数
	static Scroll scroll;
	static bool oldScrollFlag;
private:
	static DrawPolygon* draw;
	static std::map<Vector3, Area> rooms; //各部屋の情報
	static Vector3 nowRoom;    //プレイヤーが現在いる部屋
	static Vector3 oldRoomPos; //プレイヤーが一つ前にいた部屋
	static Vector3 moveDir;    //部屋移動した方向

public: //静的メンバ関数
	// 静的初期化処理
	static void StaticInit(DrawPolygon* const draw);

	// 部屋の追加
	static int CreateRoom(int direction = -1);
	static int CreateRoom(const Vector3& moveRoom);
	// 今いる部屋の削除
	static int DeleteRoom(int direction);
	// 部屋の削除
	static int DeleteRoom(int direction, const Vector3& roomPos);
	// 全部屋の削除
	static void AllDeleteRoom();

	static int MoveRoom(const Vector3& roomPos, const Vector3& direction = Vector3::Zero());
	static int MoveLeftRoom();
	static int MoveRightRoom();
	static int MoveFrontRoom();
	static int MoveBackRoom();
	static int MoveUpFloor();
	static int MoveDownFloor();

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

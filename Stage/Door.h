#pragma once
#include "./Header/DrawPolygon.h"
#include "./Header/EngineGeneral.h"

class Door
{
private: //エイリアス
	using Vector3 = Math::Vector3;

public: //サブクラス
	enum DoorStatus
	{
		OPEN,         //扉が開いている状態
		CLOSE,        //スイッチで開く扉
		WALL,         //壁
		KEY_CLOSE,    //鍵扉
		BREAK_WALL,   //壊せる壁
		ROOM_CREATE,  //部屋の生成(プログラム用)
		MAX,          //DoorStatusの上限(プログラム用)
		ENTRANCE = -1 //ダンジョンの入口
	};

private: //静的メンバ変数
	static DrawPolygon* draw;
	static int door_obj;  //ドアのオブジェクト
	static int keyDoor;   //鍵ドアのオブジェクト
	static int breakWall; //壊せる壁のオブジェクト

public: //静的メンバ関数
	// 初期化
	static void StaticInit(DrawPolygon* draw);

private: //メンバ変数
	Vector3 size;
	DoorStatus status;

public: //メンバ関数
	Door();
	~Door();

	// 初期化
	void Init(const Vector3& size, DoorStatus status);
	// 描画
	void Draw(const Vector3& offset);

	void Open() { if (status == DoorStatus::CLOSE) status = DoorStatus::OPEN; }
	void KeyOpen() { if (status == DoorStatus::KEY_CLOSE) status = DoorStatus::OPEN; }

	// 扉の状態の取得
	DoorStatus GetStatus() { return status; }

	// 扉の状態の設定
	void SetStatus(DoorStatus status)
	{
		if(status == DoorStatus::MAX) this->status = DoorStatus::OPEN;
		this->status = status;
	}
};

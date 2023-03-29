#pragma once
#include "./Header/DrawPolygon.h"
#include "./Header/EngineGeneral.h"
#include "./Math/Easing.h"

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

public: //定数
	static const float DOOR_WIDTH; //ドアの横幅
	static const float DEFAULT_DOOR_POS; //ドアが閉じている時のz軸の座標
	static const float DOWN_DOOR_POS;    //ドアが開いている時のz軸の座標

private: //静的メンバ変数
	static DrawPolygon* draw;
	static int door_obj;  //ドアのオブジェクト
	static int keyDoor;   //鍵ドアのオブジェクト
	static int breakWall; //壊せる壁のオブジェクト

public: //静的メンバ関数
	// 初期化
	static void StaticInit(DrawPolygon* draw);

private: //メンバ変数
	Vector3 pos;
	Vector3 size;
	DoorStatus status;
	Math::Ease ease;

public: //メンバ関数
	Door();
	~Door();

	// 初期化
	void Init(const Vector3& pos, const Vector3& size, DoorStatus status);
	// 描画
	void Draw(const Vector3& offset);

	void OpenEaseInit();
	void CloseEaseInit();
	void EaseUpdate();

	void Open();
	void KeyOpen();
	void BreakWall();

	// 扉のサイズの取得
	const Vector3 GetPos() const { return pos; }
	// 扉のサイズの取得
	const Vector3 GetSize() const { return size; }
	// 扉の状態の取得
	DoorStatus GetStatus() const { return status; }

	// 扉の状態の設定
	void SetStatus(DoorStatus status)
	{
		if (status == DoorStatus::OPEN || status == DoorStatus::MAX)
		{
			this->pos.z = DOWN_DOOR_POS;
			this->status = DoorStatus::OPEN;
		}
		else
		{
			this->pos.z = DEFAULT_DOOR_POS;
			this->status = status;
		}
	}
};

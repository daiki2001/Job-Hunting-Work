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
		OPEN,
		CLOSE,
		WALL,
		KEY_CLOSE,
		ENTRANCE = -1
	};

private: //静的メンバ変数
	static DrawPolygon* draw;
	static int door_obj; //ドアのオブジェクト
	static int keyDoor; //鍵ドアのオブジェクト

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
	void Init(const Vector3& size, const DoorStatus& status);
	// 描画
	void Draw(const Vector3& offset);

	void Open() { if (status == DoorStatus::CLOSE) status = DoorStatus::OPEN; }
	void KeyOpen() { if (status == DoorStatus::KEY_CLOSE) status = DoorStatus::OPEN; }

	DoorStatus GetStatus() { return status; }
};

#pragma once
#include "./Header/DrawPolygon.h"
#include "./Header/EngineGeneral.h"

class Door
{
public: //サブクラス
	enum DoorStatus
	{
		OPEN,
		CLOSE,
		WALL,
		ENTRANCE = -1
	};

private: //静的メンバ変数
	static DrawPolygon* draw;
	static int door_obj; //ドアのオブジェクト

public: //静的メンバ関数
	// 初期化
	static void StaticInit(DrawPolygon* draw);

private: //メンバ変数
	Math::Vector3 size;
	DoorStatus status;

public: //メンバ関数
	Door();
	~Door() = default;

	// 初期化
	void Init(const Math::Vector3& size, const DoorStatus& status);
	// 描画
	void Draw(const Math::Vector3& offset);

	void Open() { if (status == DoorStatus::CLOSE)status = DoorStatus::OPEN; }

	DoorStatus GetStatus() { return status; }
};

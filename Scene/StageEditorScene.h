#pragma once
#include "./Header/BaseScene.h"
#include "../Stage/Stage.h"
#include "../InputManager.h"

class StageEditorScene : public BaseScene
{
public: //定数
	static const int STAGE_WIDTH = 15;
	static const int STAGE_HEIGHT = 7;
	static const std::wstring resourcesDir;

public: //サブクラス
	// カーソルがどこを指しているか
	enum CursorState
	{
		BLOCKS,                               //何かしらのブロック
		DOOR_UP = Area::DoorNum::UP + 1,      //上のドア
		DOOR_DOWN = Area::DoorNum::DOWN + 1,  //下のドア
		DOOR_LEFT = Area::DoorNum::LEFT + 1,  //左のドア
		DOOR_RIGHT = Area::DoorNum::RIGHT + 1 //右のドア
	};

private: //静的メンバ変数
	static InputManager* inputMgr;
	static Stage* stage;

private: //メンバ変数
	int mapArray[STAGE_WIDTH * STAGE_HEIGHT];
	int mapIndex;
	int blockIndex; //選んでるブロックの種類
	int doorIndex;  //選んでるドア・壁の種類
	CursorState cursorState;

	int background; //背景画像
	int cursor; //カーソル画像

public: //メンバ関数
	StageEditorScene(DrawPolygon* draw, SceneChenger* sceneChenger);
	~StageEditorScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;
private:
	// カーソル移動
	void CursorMove();
	void CursorMove(int createRoomDir);
	// ブロック選択
	void SelectBlock();
	// ドア・壁選択
	void SelectDoor();

	// カーソルを左に移動
	void CursorMoveLeft();
	// カーソルを右に移動
	void CursorMoveRight();
	// カーソルを上に移動
	void CursorMoveUp();
	// カーソルを下に移動
	void CursorMoveDown();
};

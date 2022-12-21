#pragma once
#include "./Scene/BaseScene.h"
#include "../Stage/Stage.h"
#include "../UndoRedo/FuncRedoUndo.h"
#include "../InputManager.h"

class FuncRedoUndo;

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
	bool isRoute;   //迷いの森の道を編集出来るかどうか
	int routeIndex; //選んでる迷いの森の道のインデックス
	CursorState cursorState;

	FuncRedoUndo redoUndo;

	int background; //背景画像
	int cursor; //カーソル画像
	int frame; //フレーム画像
	int cross; //×の画像

public: //メンバ関数
	StageEditorScene(SceneChanger* sceneChanger);
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
	// 迷いの森の道の設定
	void RouteSetting();

	// カーソルを左に移動
	void CursorMoveLeft();
	// カーソルを右に移動
	void CursorMoveRight();
	// カーソルを上に移動
	void CursorMoveUp();
	// カーソルを下に移動
	void CursorMoveDown();

	void DrawUIBox(float posX, float posY, float size, float angle, int graphHandle, bool flag);
};

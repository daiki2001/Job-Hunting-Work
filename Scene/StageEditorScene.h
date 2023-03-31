#pragma once
#include "./Scene/BaseScene.h"
#include "../Stage/Stage.h"
#include "../UndoRedo/FuncRedoUndo.h"
#include "./Input/GameInput.h"

class FuncRedoUndo;

class StageEditorScene : public BaseScene
{
private: //エイリアス
	using XMFLOAT4 = DirectX::XMFLOAT4;

public: //定数
	static const int STAGE_WIDTH = 15;
	static const int STAGE_HEIGHT = 7;
	static const std::wstring resourcesDir;
	static const int FRAME_SIZE;  //枠の大きさ
	static const int FRAME_BLANK; //枠と枠の間隔

public: //サブクラス
	// カーソルがどこを指しているか
	enum CursorState
	{
		BLOCKS,                               //何かしらのブロック
		DOOR_UP = Area::DoorNum::TOP + 1,      //上のドア
		DOOR_DOWN = Area::DoorNum::BOTTOM + 1,  //下のドア
		DOOR_LEFT = Area::DoorNum::LEFT + 1,  //左のドア
		DOOR_RIGHT = Area::DoorNum::RIGHT + 1 //右のドア
	};

private: //静的メンバ変数
	static GameInput* inputMgr;
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

	int cursor; //カーソル画像
	int frame; //フレーム画像
	int cross; //×の画像

public: //メンバ関数
	StageEditorScene(SceneChanger* sceneChanger);
	~StageEditorScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;
	void UIDraw() override;
private:
	// 配置されたブロックを消す
	void EraseBlock();

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

	void DrawUIBox(float posX, float posY, float size, float angle, int graphHandle, const XMFLOAT4& color, bool flag);
	void DrawSelectBlockUI(float offsetX = 0.0f, float offsetY = 0.0f);
	void DrawSelectDoorUI(float offsetX = 0.0f, float offsetY = 0.0f);
};

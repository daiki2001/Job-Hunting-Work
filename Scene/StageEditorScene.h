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
		BLOCKS,    //何かしらのブロック
		DOOR_UP,   //上のドア
		DOOR_DOWN, //下のドア
		DOOR_LEFT, //左のドア
		DOOR_RIGHT //右のドア
	};

private: //静的メンバ変数
	static InputManager* inputMgr;
	static Stage* stage;

private: //メンバ変数
	int mapArray[STAGE_WIDTH * STAGE_HEIGHT];
	int mapIndex;
	int blockIndex;
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
	// ブロック選択
	void SelectBlock();

	// カーソルを左に移動
	void CursorMoveLeft();
	// カーソルを右に移動
	void CursorMoveRight();
	// カーソルを上に移動
	void CursorMoveUp();
	// カーソルを下に移動
	void CursorMoveDown();
};

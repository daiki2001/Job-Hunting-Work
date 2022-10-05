#pragma once
#include "./Header/BaseScene.h"
#include "../Stage/Stage.h"

class StageEditorScene : public BaseScene
{
public: //定数
	static const int STAGE_WIDTH = 15;
	static const int STAGE_HEIGHT = 7;
	static const std::wstring resourcesDir;

private: //静的メンバ変数
	static Stage* stage;

private: //メンバ変数
	int mapArray[STAGE_WIDTH * STAGE_HEIGHT];
	int mapIndex;
	int blockIndex;

	int background; //背景画像
	int cursor; //カーソル画像

public: //メンバ関数
	StageEditorScene(DrawPolygon* draw, SceneChenger* sceneChenger);
	~StageEditorScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;
};

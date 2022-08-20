#pragma once
#include "./Header/BaseScene.h"

class StageEditorScene : public BaseScene
{
public: //定数
	static const std::wstring resourcesDir;

public: //メンバ関数
	StageEditorScene(DrawPolygon* draw, SceneChenger* sceneChenger);
	~StageEditorScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: //メンバ変数
	int background; //背景画像
};

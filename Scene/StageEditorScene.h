#pragma once
#include "./Header/BaseScene.h"

class StageEditorScene : public BaseScene
{
public: //’è”
	static const std::wstring resourcesDir;

public: //ƒƒ“ƒoŠÖ”
	StageEditorScene(DrawPolygon* draw, SceneChenger* sceneChenger);
	~StageEditorScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: //ƒƒ“ƒo•Ï”
	int background; //”wŒi‰æ‘œ
};

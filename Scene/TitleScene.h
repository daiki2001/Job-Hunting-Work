#pragma once
#include "./Header/BaseScene.h"
#include <string>

class TitleScene : public BaseScene
{
public: //’è”
	static const std::wstring titleResourcesDir;

public: //ƒƒ“ƒoŠÖ”
	TitleScene(SceneChenger* sceneChenger);
	~TitleScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: //ƒƒ“ƒo•Ï”
	int background; //”wŒi‰æ‘œ
};

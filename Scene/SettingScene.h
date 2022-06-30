#pragma once
#include "./Header/BaseScene.h"

class SettingScene : public BaseScene
{
public: //’è”
	static const std::wstring settingResourcesDir;

public: //ƒƒ“ƒoŠÖ”
	SettingScene(SceneChenger* sceneChenger);
	~SettingScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: //ƒƒ“ƒo•Ï”
	int background; //”wŒi‰æ‘œ
};

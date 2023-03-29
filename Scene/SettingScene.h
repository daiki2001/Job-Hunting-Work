#pragma once
#include "./Scene/BaseScene.h"

class SettingScene : public BaseScene
{
public: //定数
	static const std::wstring settingResourcesDir;

public: //メンバ関数
	SettingScene(SceneChanger* sceneChanger);
	~SettingScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;
	void BGDraw() override;

private: //メンバ変数
	int background; //背景画像
};

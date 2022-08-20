#pragma once
#include "./Header/BaseScene.h"

class SettingScene : public BaseScene
{
public: //定数
	static const std::wstring settingResourcesDir;

public: //メンバ関数
	SettingScene(DrawPolygon* draw, SceneChenger* sceneChenger);
	~SettingScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: //メンバ変数
	int background; //背景画像
};

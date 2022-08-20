#pragma once
#include "./Header/BaseScene.h"
#include <string>

class TitleScene : public BaseScene
{
public: //定数
	static const std::wstring titleResourcesDir;

public: //メンバ関数
	TitleScene(DrawPolygon* draw, SceneChenger* sceneChenger);
	~TitleScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: //メンバ変数
	int background; //背景画像
};

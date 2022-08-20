#pragma once
#include "./Header/BaseScene.h"

class GameScene :  public BaseScene
{
public: //定数
	static const std::wstring gameResourcesDir;

public: //メンバ関数
	GameScene(DrawPolygon* draw, SceneChenger* sceneChenger);
	~GameScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: //メンバ変数
	int background; //背景画像
};

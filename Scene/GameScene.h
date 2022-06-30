#pragma once
#include "./Header/BaseScene.h"

class GameScene :  public BaseScene
{
public: //’è”
	static const std::wstring gameResourcesDir;

public: //ƒƒ“ƒoŠÖ”
	GameScene(SceneChenger* sceneChenger);
	~GameScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;

private: //ƒƒ“ƒo•Ï”
	int background; //”wŒi‰æ‘œ
};

#pragma once
#include "./Header/BaseScene.h"
#include "../Player/Player.h"
#include "../Stage/Stage.h"

class GameScene :  public BaseScene
{
public: //定数
	static const std::wstring gameResourcesDir;
	static const std::string stageDir;
	
private: //静的メンバ変数
	static Player* player;
	static Stage* stage;
	static bool isClear;

private: //メンバ変数
	int background; //背景画像
	int clear;

public: //メンバ関数
	GameScene(DrawPolygon* draw, SceneChenger* sceneChenger);
	~GameScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;
private:
	void Reset();
};

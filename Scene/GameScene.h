﻿#pragma once
#include "./Scene/BaseScene.h"
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
	int backgroundObj; //背景画像を貼り付けるためのオブジェクト
	int clear;

public: //メンバ関数
	GameScene(SceneChanger* sceneChanger);
	~GameScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;
	void BGDraw() override;
	void UIDraw() override;
private:
	void Reset();
};

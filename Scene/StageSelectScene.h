#pragma once
#include "./Scene/BaseScene.h"
#include "./Header/EngineGeneral.h"
#include "../InputManager.h"

class StageSelectScene : public BaseScene
{
public: //エイリアス
	using Vector3 = Math::Vector3;

public: //サブクラス
	enum StageStatus
	{
		TITLE = 0,
		USER = -1,
		MAX = 1
	};

public: //定数
	static const std::wstring resourcesDir;
	static const std::string stageDir;

private: //静的メンバ変数
	static InputManager* inputMgr;

private: //メンバ変数
	std::string stagePath;
	int stageIndex;

	int background; //背景画像
	int cave;       //洞窟のモデルデータ
	int title;      //「TITLE」の文字画像
	int stage;      //「Stage」の文字画像
	int userStage;  //「User Stage」の文字画像
	int arrow;      //矢印の画像
	int number;     //数字の文字画像

public: //メンバ関数
	StageSelectScene(SceneChanger* sceneChanger);
	~StageSelectScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;
private:
};

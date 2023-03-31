#pragma once
#include "./Scene/BaseScene.h"
#include <string>

class TitleScene : public BaseScene
{
public: //定数
	static const std::wstring titleResourcesDir;

public: //サブクラス
	enum ButtonIndex
	{
		START,
		EDITER
	};

private: //メンバ変数
	ButtonIndex buttonIndex;

	int background; //背景画像
	int buttonBack; //ボタンの背景
	int start;
	int editer;

public: //メンバ関数
	TitleScene(SceneChanger* sceneChanger);
	~TitleScene() override;

	void Init() override;
	void Update() override;
	void Draw() override;
	void BGDraw() override;
	void UIDraw() override;
};

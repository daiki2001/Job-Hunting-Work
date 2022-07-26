#pragma once

class SceneChenger
{
public: // サブクラス
	enum Scene
	{
		Title,
		Game,
		Setting
	};

public: // メンバ関数
	SceneChenger() {}
	~SceneChenger() {}

	virtual void SceneChenge(const SceneChenger::Scene scene, const bool stackClear) = 0;
	virtual int PopScene() = 0;
};

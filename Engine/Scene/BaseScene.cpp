﻿#include "BaseScene.h"
#include "./Header/DirectXInit.h"

DrawPolygon* BaseScene::draw = nullptr;
SceneChangeAnimation BaseScene::changeAnimation = nullptr;

BaseScene::BaseScene(SceneChanger* sceneChanger) :
	sceneChanger(sceneChanger),
	isSceneDest(false),
	nextScene(SceneChanger::Scene::Title)
{
}

BaseScene::~BaseScene()
{
	sceneChanger = nullptr;
}

void BaseScene::ChengeTitleScene(bool flag)
{
	if (flag)
	{
		isSceneDest = true;
		nextScene = SceneChanger::Scene::Title;
		changeAnimation.Start();
	}

	if (isSceneDest)
	{
		if (changeAnimation.GetChange())
		{
			sceneChanger->SceneChange(nextScene, true);
		}
	}
}

#include "./Header/Scenemanager.h"
#include "./Header/Camera.h"
#include "./Header/DirectXInit.h"

#include "./Scene/EngineTestScene.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/SettingScene.h"

#include "./Header/Error.h"

SceneManager::SceneManager()
{
	if (DirectXInit::EngineDebug)
	{
		sceneStack.push(std::make_shared<EngineTestScene>(this));
	}
	else
	{
		sceneStack.push(std::make_shared<TitleScene>(this));
	}
}

void SceneManager::Loop() const
{
	sceneStack.top()->Update();
	Camera::Update();
	sceneStack.top()->Draw();
}

void SceneManager::SceneChenge(const SceneChenger::Scene scene, const bool stackClear)
{
	using namespace std;

	if (stackClear)
	{
		while (sceneStack.empty() == false)
		{
			sceneStack.pop();
		}
	}

	switch (scene)
	{
	case SceneChenger::Scene::Title:
		sceneStack.push(make_shared<TitleScene>(this));
		break;
	case SceneChenger::Scene::Game:
		sceneStack.push(make_shared<GameScene>(this));
		break;
	case SceneChenger::Scene::Setting:
		sceneStack.push(make_shared<SettingScene>(this));
		break;
	default:
		Engine::ErrorLog("ë∂ç›ÇµÇ»Ç¢ÉVÅ[ÉìÇ™åƒÇŒÇÍÇ‹ÇµÇΩ\n", true);
		break;
	}
}

int SceneManager::PopScene()
{
	if (sceneStack.size() <= 1)
	{
		return Engine::FUNCTION_ERROR;
	}

	sceneStack.pop();

	return 0;
}

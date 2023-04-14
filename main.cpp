#include "./Header/DirectXInit.h"
#include "./Header/EngineGeneral.h"
#include "./Scene/SceneManager.h"
#include "./Input/Input.h"

/*ウィンドウサイズ*/
constexpr int window_width = 1280; //横幅
constexpr int window_height = 768; //縦幅

constexpr WCHAR title[] = L"猫の記憶"; //タイトル

constexpr float clearColor[] = { 0.1f, 0.1f, 0.15f, 1.0f }; //背景色

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	DirectXInit* w = DirectXInit::GetInstance();
	EngineInit(title, clearColor, window_width, window_height);

	SceneManager sceneManager;

	while (true)
	{
		if (w->WindowMessage() == -1) { break; }

		/* 更新 */
		Input::Update();

		sceneManager.Loop();

		if (Input::IsKey(DIK_ESCAPE)) { break; }
	}

	w->WindowEnd();

	return 0;
}
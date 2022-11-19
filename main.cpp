#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/Camera.h"
#include "./Header/Input.h"
#include "./Scene/SceneManager.h"
#include "./Header/EngineGeneral.h"
#include "./Header/Parameter.h"

/*ウィンドウサイズ*/
constexpr int window_width = 1280; //横幅
constexpr int window_height = 768; //縦幅

constexpr WCHAR title[] = L"猫の記憶"; //タイトル

constexpr float clearColor[] = { 0.1f, 0.875f, 0.875f, 1.0f }; //背景色

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	DirectXInit* w = DirectXInit::GetInstance();

#ifdef _DEBUG
	DirectXInit::EngineDebug = true;
#endif // _DEBUG

	w->title = title;
	for (UINT i = 0; i < 4; i++)
	{
		w->clearColor[i] = clearColor[i];
	}
	w->SetWindowSize(window_width, window_height);
	w->Init();

	DrawPolygon draw;
	Camera::Init();
	Input::Init();

	int whiteTex = draw.LoadTextrue((StringToWString(engineResourcesDir) + L"white1x1.png").c_str());
	Parameter::Set("white1x1", whiteTex);

	SceneManager sceneManager(&draw);

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
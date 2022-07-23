#include "EngineTestScene.h"
#include "./Header/DirectXInit.h"
#include "./Header/ShaderManager.h"
#include "./Header/Input.h"
#include "./Header/Camera.h"

EngineTestScene::EngineTestScene(SceneChenger* sceneChenger) :
	BaseScene(sceneChenger),
	fbxLoader(FbxLoader::GetInstance()),
	background(FUNCTION_ERROR),
	fbxModel(FUNCTION_ERROR),
	postEffect{}
{
	Init();
	postEffect.Init();
}

EngineTestScene::~EngineTestScene()
{
	fbxLoader->Finalize();
}

void EngineTestScene::Init()
{
	using namespace DirectX;
	using namespace Math;

	fbxLoader->Init();
	fbxModel = fbxLoader->LoadModelFromFile("./Resources/boneTest.fbx");
	fbxLoader->GetModel(fbxModel)->Init();

	// 画像の読み込み
	background = draw.LoadTextrue(L"./Resources/background.png");

	Camera::targetRadius = 10.0f;
	Camera::longitude = DEGREE_F * (0.0f);
	Camera::latitude = DEGREE_F * (0.0f);

	Camera::pos = { 0.0f, 0.0f, -10.0f };
	Camera::target = { 0.0f, 2.5f, 0.0f };
	Camera::upVec = { 0.0f, 1.0f, 0.0f };

	fbxLoader->GetModel(fbxModel)->pos = { 0.0f, 0.0f, 0.0f };
	fbxLoader->GetModel(fbxModel)->PlayAnimation();
}

void EngineTestScene::Update()
{
	using namespace DirectX;
	using namespace Math;
	using namespace Collision;

	if (Input::IsKey(DIK_R))
	{
		Camera::targetRadius = 20.0f;
		Camera::longitude = DEGREE_F * (0.0f);
		Camera::latitude = DEGREE_F * (0.0f);
	}

	if (Input::IsKey(DIK_A))
	{
		Camera::longitude -= DEGREE_F;
		if (Camera::longitude <= DEGREE_F * 0.0)
		{
			Camera::longitude += DEGREE_F * 360;
		}
	}
	if (Input::IsKey(DIK_D))
	{
		Camera::longitude += DEGREE_F;
		if (Camera::longitude >= DEGREE_F * 360.0)
		{
			Camera::longitude -= DEGREE_F * 360;
		}
	}
	if (Input::IsKey(DIK_W))
	{
		Camera::latitude += DEGREE_F;
		if (Camera::latitude >= DEGREE_F * 90.0f)
		{
			Camera::latitude = DEGREE_F * 90.0f - DEGREE_F;
		}
	}
	if (Input::IsKey(DIK_S))
	{
		Camera::latitude -= DEGREE_F;
		if (Camera::latitude <= DEGREE_F * -90.0f)
		{
			Camera::latitude = -(DEGREE_F * 90.0f - DEGREE_F);
		}
	}

	if (Input::IsKey(DIK_PGUP))
	{
		Camera::targetRadius -= 1.0f;
		if (Camera::targetRadius < 10.0f)
		{
			Camera::targetRadius = 10.0f;
		}
	}
	if (Input::IsKey(DIK_PGDN))
	{
		Camera::targetRadius += 1.0f;
	}

	Camera::pos.x = cosf(Camera::longitude) * cosf(Camera::latitude);
	Camera::pos.y = sinf(Camera::latitude);
	Camera::pos.z = sinf(Camera::longitude) * cosf(Camera::latitude);
	Camera::pos *= Camera::targetRadius;
	Camera::pos += Camera::target;
	Camera::SetCamera(Camera::pos, Camera::target, Camera::upVec);

	fbxLoader->GetModel(fbxModel)->Update();
}

void EngineTestScene::Draw()
{
	using namespace DirectX;
	using namespace Math;

	DirectXInit* w = DirectXInit::GetInstance();

	postEffect.PreDraw();
	draw.SetDrawBlendMode(BLENDMODE_ALPHA);
	ShaderManager::blendMode = ShaderManager::BlendMode::ALPHA;

	fbxLoader->GetModel(fbxModel)->Draw();

	postEffect.PostDraw();

	w->ClearScreen();
	// 背景
	draw.DrawTextrue(
		0.0f,
		0.0f,
		static_cast<float>(w->windowWidth),
		static_cast<float>(w->windowHeight),
		0.0f,
		background,
		XMFLOAT2(0.0f, 0.0f)
	);

	postEffect.Draw();
	w->ScreenFlip();

	// ループの終了処理
	draw.PolygonLoopEnd();
}

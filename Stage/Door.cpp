#include "Door.h"

namespace
{
int debugTex = FUNCTION_ERROR;
int wallTex = FUNCTION_ERROR;
int entranceLight = FUNCTION_ERROR;
}

DrawPolygon* Door::draw = nullptr;
int Door::door_obj = FUNCTION_ERROR;

void Door::StaticInit(DrawPolygon* draw)
{
	Door::draw = draw;
	door_obj = Door::draw->Create3Dbox(1.0f, 1.0f, 1.0f);
	debugTex = Door::draw->LoadTextrue(L"./Resources/Engine/white1x1.png");
	wallTex = Door::draw->LoadTextrue(StringToWString(resourcesDirectory + "Game/wall.png").c_str());
	entranceLight = Door::draw->CreateCircle(1.0f, 16);
}

Door::Door() :
	size(Vector3(1.0f, 1.0f, 1.0f)),
	status(DoorStatus::OPEN)
{
}

void Door::Init(const Vector3& size, const DoorStatus& status)
{
	this->size = size;
	this->status = status;
}

void Door::Draw(const Vector3& offset)
{
	using namespace Math;

	DirectDrawing::ChangeOBJShader();
	switch (status)
	{
	case DoorStatus::CLOSE:
		draw->Draw(door_obj, offset, Identity(), size, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				   debugTex);
		break;
	case DoorStatus::WALL:
		draw->Draw(door_obj, offset, Identity(), size, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				   wallTex);
		break;
	case DoorStatus::ENTRANCE:
		draw->Draw(entranceLight, offset, Identity(), Vector3(1.0f, 1.0f, 1.0f),
				   DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), debugTex);
		break;
	case DoorStatus::OPEN:
	default:
		break;
	}
}

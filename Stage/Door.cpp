#include "Door.h"

namespace
{
int debugTex = FUNCTION_ERROR;
}

DrawPolygon* Door::draw = nullptr;
int Door::door_obj = FUNCTION_ERROR;

void Door::StaticInit(DrawPolygon* draw)
{
	Door::draw = draw;
	door_obj = Door::draw->Create3Dbox(1.0f, 1.0f, 1.0f);
	debugTex = Door::draw->LoadTextrue(L"./Resources/Engine/white1x1.png");
}

Door::Door() :
	size(Math::Vector3(1.0f, 1.0f, 1.0f)),
	status(DoorStatus::OPEN)
{
}

void Door::Init(const Math::Vector3& size, const DoorStatus& status)
{
	this->size = size;
	this->status = status;
}

void Door::Draw(const Math::Vector3& offset)
{
	DirectDrawing::ChangeOBJShader();
	switch (status)
	{
	case DoorStatus::CLOSE:
		draw->Draw(door_obj, offset, Math::Identity(), size, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), debugTex);
		break;
	case DoorStatus::WALL:
		draw->Draw(door_obj, offset, Math::Identity(), size, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), debugTex);
		break;
	case DoorStatus::OPEN:
	case DoorStatus::ENTRANCE:
	default:
		break;
	}
}

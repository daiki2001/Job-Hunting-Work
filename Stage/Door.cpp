#include "Door.h"
#include "Area.h"
#include "./Header/Parameter.h"

namespace
{
int debugTex = FUNCTION_ERROR;
int entranceLight = FUNCTION_ERROR;
}

DrawPolygon* Door::draw = nullptr;
int Door::door_obj = FUNCTION_ERROR;
int Door::keyDoor = FUNCTION_ERROR;

void Door::StaticInit(DrawPolygon* draw)
{
	Door::draw = draw;

	if (door_obj == FUNCTION_ERROR)
	{
		door_obj = Door::draw->Create3Dbox(1.0f, 1.0f, 1.0f);
	}
	if (keyDoor == FUNCTION_ERROR)
	{
		keyDoor = Door::draw->CreateOBJModel("./Resources/Game/Wall/KeyWall.obj", "./Resources/Game/Wall/");
	}
	if (debugTex == FUNCTION_ERROR)
	{
		debugTex = Parameter::Get("white1x1");
	}
	if (entranceLight == FUNCTION_ERROR)
	{
		entranceLight = Door::draw->CreateCircle(1.0f, 16);
	}
}

Door::Door() :
	size(Vector3(1.0f, 1.0f, 1.0f)),
	status(DoorStatus::OPEN)
{
}

Door::~Door()
{
	//door_obj = FUNCTION_ERROR;
	//keyDoor = FUNCTION_ERROR;
	//debugTex = FUNCTION_ERROR;
	//entranceLight = FUNCTION_ERROR;
}

void Door::Init(const Vector3& size, DoorStatus status)
{
	this->size = size;
	this->status = status;
}

void Door::Draw(const Vector3& offset)
{
	using namespace Math;

	if (status == WALL || status == KEY_CLOSE)
	{
		DirectDrawing::ChangeMaterialShader();
	}
	else
	{
		DirectDrawing::ChangeOBJShader();
	}

	switch (status)
	{
	case DoorStatus::CLOSE:
		draw->Draw(door_obj, offset, Identity(), size, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				   debugTex);
		break;
	case DoorStatus::WALL:
		draw->DrawOBJ(Area::GetWallObj(), offset, Identity(), size);
		break;
	case DoorStatus::KEY_CLOSE:
		draw->DrawOBJ(keyDoor, offset, rotateX(PI_F), size);
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

#include "Door.h"
#include "Area.h"
#include "./Header/Parameter.h"

namespace
{
int debugTex = FUNCTION_ERROR;
int entranceLight = FUNCTION_ERROR;
}

float Door::DOOR_WIDTH = 3.0f;
DrawPolygon* Door::draw = nullptr;
int Door::door_obj = FUNCTION_ERROR;
int Door::keyDoor = FUNCTION_ERROR;
int Door::breakWall = FUNCTION_ERROR;

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
	if (breakWall == FUNCTION_ERROR)
	{
		breakWall = Door::draw->CreateOBJModel("./Resources/Game/Wall/BreakWall.obj", "./Resources/Game/Wall/");
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
}

void Door::Init(const Vector3& size, DoorStatus status)
{
	this->size = size;
	this->status = status;
}

void Door::Draw(const Vector3& offset)
{
	using namespace Math;

	if (status == WALL || status == KEY_CLOSE || status == BREAK_WALL)
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
	case DoorStatus::BREAK_WALL:
		draw->DrawOBJ(breakWall, offset, rotateX(PI_F), size);
		break;
	case DoorStatus::ENTRANCE:
		draw->Draw(entranceLight, offset, Identity(), Vector3(1.0f, 1.0f, 1.0f),
				   DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f), debugTex);
		break;
	case DoorStatus::OPEN:
	{
		static const float halfWidth = floorf(Door::DOOR_WIDTH / 2.0f);

		for (float i = 0; i < Door::DOOR_WIDTH; i += 1.0f)
		{
			if (size.x == Door::DOOR_WIDTH)
			{
				BlockType::FloorDraw(Vector3((i - halfWidth) + Area::INIT_CAMERA.x, 0.0f, 0.0f) + offset);
			}
			else
			{
				BlockType::FloorDraw(Vector3(0.0f, (i - halfWidth) + Area::INIT_CAMERA.y, 0.0f) + offset);
			}
		}
		break;
	}
	default:
		break;
	}
}

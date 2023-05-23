#include "Door.h"
#include "Area.h"
#include "./Header/DrawPolygon.h"
#include "./Header/Parameter.h"

namespace
{
int debugTex = FUNCTION_ERROR;
int entranceLight = FUNCTION_ERROR;
}

const float Door::DOOR_WIDTH = 3.0f;
const float Door::DEFAULT_DOOR_POS = -0.5f;
const float Door::DOWN_DOOR_POS = 1.5f;
int Door::door_obj = FUNCTION_ERROR;
int Door::keyDoor = FUNCTION_ERROR;
int Door::breakWall = FUNCTION_ERROR;

void Door::StaticInit()
{
	auto draw = Library::DrawPolygon::GetInstance();

	if (door_obj == FUNCTION_ERROR)
	{
		door_obj = draw->Create3Dbox(1.0f, 1.0f, 1.0f);
	}
	if (keyDoor == FUNCTION_ERROR)
	{
		keyDoor = draw->CreateOBJModel("./Resources/Game/Wall/KeyWall.obj", "./Resources/Game/Wall/");
	}
	if (breakWall == FUNCTION_ERROR)
	{
		breakWall = draw->CreateOBJModel("./Resources/Game/Wall/BreakWall.obj", "./Resources/Game/Wall/");
	}
	if (debugTex == FUNCTION_ERROR)
	{
		debugTex = Parameter::Get("white1x1");
	}
	if (entranceLight == FUNCTION_ERROR)
	{
		entranceLight = draw->CreateCircle(1.0f, 16);
	}
}

Door::Door() :
	pos(Vector3::Zero()),
	size(Vector3(1.0f, 1.0f, 1.0f)),
	status(DoorStatus::OPEN),
	ease{}
{
}

Door::~Door()
{
}

void Door::Init(const Vector3& pos, const Vector3& size, DoorStatus status)
{
	this->pos = pos;
	this->size = size;
	this->status = status;

	if (this->status == DoorStatus::OPEN)
	{
		this->pos.z = DOWN_DOOR_POS;
	}
	else
	{
		this->pos.z = DEFAULT_DOOR_POS;
	}
}

void Door::OpenEaseInit()
{
	Vector3 start = Vector3(pos.x, pos.y, DEFAULT_DOOR_POS);
	Vector3 end = Vector3(pos.x, pos.y, DOWN_DOOR_POS);

	ease.isAlive = true;
	ease.time = 0.0f;
	ease.start = start;
	ease.end = end;
}

void Door::CloseEaseInit()
{
	Vector3 start = Vector3(pos.x, pos.y, DOWN_DOOR_POS);
	Vector3 end = Vector3(pos.x, pos.y, DEFAULT_DOOR_POS);

	ease.isAlive = true;
	ease.time = 0.0f;
	ease.start = start;
	ease.end = end;
}

void Door::EaseUpdate()
{
	static float addTime = 0.2f;

	if (ease.isAlive == false) return;

	ease.time += addTime;
	pos = Math::Lerp(ease.start, ease.end, ease.time);
	if (ease.time >= 1.0f) ease.isAlive = false;
}

void Door::Draw(const Vector3& offset)
{
	using namespace Math;
	auto draw = Library::DrawPolygon::GetInstance();

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
		draw->Draw(door_obj, pos + offset, Identity(), size, Color::AddAlphaValue(Color::WHITE, 1.0f),
				   debugTex);
		break;
	case DoorStatus::WALL:
		draw->DrawOBJ(Area::GetWallObj(), pos + offset, Identity(), size);
		break;
	case DoorStatus::KEY_CLOSE:
		draw->DrawOBJ(keyDoor, pos + offset, rotateX(PI_F), size);
		break;
	case DoorStatus::BREAK_WALL:
		draw->DrawOBJ(breakWall, pos + offset, rotateX(PI_F), size);
		break;
	case DoorStatus::ENTRANCE:
		draw->Draw(entranceLight, pos + offset, Identity(), Vector3(1.0f, 1.0f, 1.0f),
				   Color::AddAlphaValue(Color::WHITE, 0.5f), debugTex);
		break;
	case DoorStatus::OPEN:
		if (pos.z < DOWN_DOOR_POS)
		{
			draw->Draw(door_obj, pos + offset, Identity(), size, Color::AddAlphaValue(Color::WHITE, 1.0f),
					   debugTex);
		}
		//else
		//{
		//	static const float halfNum = floorf((Door::DOOR_WIDTH + 2.0f) / 2.0f);
		//	float symbol = 0.0f;

		//	if (((pos + offset - Area::INIT_CAMERA).x > 0) || ((pos + offset - Area::INIT_CAMERA).y > 0))
		//	{
		//		symbol = +1.0f;
		//	}
		//	else if (((pos + offset - Area::INIT_CAMERA).x < 0) || ((pos + offset - Area::INIT_CAMERA).y < 0))
		//	{
		//		symbol = -1.0f;
		//	}

		//	for (float i = 0; i < Door::DOOR_WIDTH + 2.0f; i += 1.0f)
		//	{
		//		for (float j = 0; j < 2.0f; j += 1.0f)
		//		{
		//			if (size.x == Door::DOOR_WIDTH)
		//			{
		//				BlockType::FloorDraw(Vector3(pos.x + (i - halfNum), pos.y + j * symbol, 0.0f) + offset);
		//			}
		//			else
		//			{
		//				BlockType::FloorDraw(Vector3(pos.x + j * symbol, pos.y + (i - halfNum), 0.0f) + offset);
		//			}
		//		}
		//	}
		//}
		break;
	default:
		break;
	}
}

void Door::Open()
{
	if (status != DoorStatus::CLOSE) return;

	pos.z = DOWN_DOOR_POS;
	status = DoorStatus::OPEN;
	OpenEaseInit();
}

void Door::KeyOpen()
{
	if (status != DoorStatus::KEY_CLOSE) return;

	status = DoorStatus::OPEN;
	pos.z = DOWN_DOOR_POS;
}

void Door::BreakWall()
{
	if (status != DoorStatus::BREAK_WALL) return;

	status = DoorStatus::OPEN;
	pos.z = DOWN_DOOR_POS;
}

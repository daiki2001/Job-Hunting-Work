#include "Bomb.h"
#include "./Stage/Stage.h"
#include "./Math/Collision/Collision.h"
#include "./Header/Parameter.h"
#include "LoadGraph.h"

namespace
{
static int object = FUNCTION_ERROR;
}

Bomb::Bomb(unsigned int maxCount, unsigned int time) :
	Item(maxCount, 0U),
	pos{},
	maxTime(time),
	nowTime(0U),
	isBomber(false),
	isAlive(false)
{
}

void Bomb::Init()
{
	if (graph == FUNCTION_ERROR)
	{
		graph = Parameter::Get(LoadGraph::BOMB.c_str());
	}
	if (object == FUNCTION_ERROR)
	{
		object = draw->CreateCircle(1.0f, 8);
	}
}

void Bomb::Update()
{
	if (isAlive == false) return;

	if (isBomber)
	{
		if (nowTime >= maxTime) isAlive = false;
	}
	else
	{
		if (nowTime >= maxTime)
		{
			isBomber = true;
			nowTime = 0;

			static const Vector3 offset = Vector3(7.0f, -3.0f, 0.0f);

			static const Vector3 upDoorPos = Vector3(0.0f, +4.5f, 0.0f) + offset;
			static const Vector3 upDoorSize = Vector3(3.0f, 2.0f, 2.0f);
			static const Vector3 downDoorPos = Vector3(0.0f, -4.5f, 0.0f) + offset;
			static const Vector3 downDoorSize = Vector3(3.0f, 2.0f, 2.0f);
			static const Vector3 leftDoorPos = Vector3(-8.5f, 0.0f, 0.0f) + offset;
			static const Vector3 leftDoorSize = Vector3(2.0f, 3.0f, 2.0f);
			static const Vector3 rightDoorPos = Vector3(+8.5f, 0.0f, 0.0f) + offset;
			static const Vector3 rightDoorSize = Vector3(2.0f, 3.0f, 2.0f);

			Stage* stage = Stage::Get();

			if (Collision::IsSphereToAABBCollision(Sphere(pos, 1.0f),
												   upDoorPos - (upDoorSize / 2.0f),
												   upDoorPos + (upDoorSize / 2.0f)) &&
				stage->GetDoorStatus(Area::DoorNum::TOP) == Door::DoorStatus::BREAK_WALL)
			{
				stage->GetArea().GetDoor(Area::DoorNum::TOP).BreakWall();
			}
			if (Collision::IsSphereToAABBCollision(Sphere(pos, 1.0f),
												   downDoorPos - (downDoorSize / 2.0f),
												   downDoorPos + (downDoorSize / 2.0f)) &&
				stage->GetDoorStatus(Area::DoorNum::BOTTOM) == Door::DoorStatus::BREAK_WALL)
			{
				stage->GetArea().GetDoor(Area::DoorNum::BOTTOM).BreakWall();
			}
			if (Collision::IsSphereToAABBCollision(Sphere(pos, 1.0f),
												   leftDoorPos - (leftDoorSize / 2.0f),
												   leftDoorPos + (leftDoorSize / 2.0f)) &&
				stage->GetDoorStatus(Area::DoorNum::LEFT) == Door::DoorStatus::BREAK_WALL)
			{
				stage->GetArea().GetDoor(Area::DoorNum::LEFT).BreakWall();
			}
			if (Collision::IsSphereToAABBCollision(Sphere(pos, 1.0f),
												   rightDoorPos - (rightDoorSize / 2.0f),
												   rightDoorPos + (rightDoorSize / 2.0f)) &&
				stage->GetDoorStatus(Area::DoorNum::RIGHT) == Door::DoorStatus::BREAK_WALL)
			{
				stage->GetArea().GetDoor(Area::DoorNum::RIGHT).BreakWall();
			}
		}
	}
	nowTime++;
}

void Bomb::Draw(int offsetX, int offsetY)
{
	if (isAlive == false) return;

	DirectDrawing::ChangeOBJShader();
	if (isBomber)
	{
		draw->Draw(object, pos, Math::Identity(), scale_xyz(1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.8f),
				   Parameter::Get("white1x1"));
	}
	else
	{
		draw->Draw(object, pos, Math::Identity(), scale_xyz(0.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				   Parameter::Get(LoadGraph::BOMB.c_str()));
	}
}

void Bomb::Set(Vector3 pos)
{
	Use();

	this->pos = pos;
	this->nowTime = 0;
	this->isBomber = false;
	this->isAlive = true;
}

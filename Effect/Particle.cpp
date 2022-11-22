#include "Particle.h"

Particle::Particle() :
	pos(0.0f, 0.0f, 0.0f),
	speed(0.0f, 0.0f, 0.0f),
	time(0),
	isAlive(false)
{
	Init();
}

void Particle::Init()
{
	pos = { 0.0f, 0.0f, 0.0f };
	speed = { 0.0f, 0.0f, 0.0f };
	time = 0;
	isAlive = false;
}

void Particle::Update(const int& maxTime)
{
	if (isAlive == false)
	{
		return;
	}

	if (time >= maxTime)
	{
		isAlive = false;
		return;
	}

	pos += speed;
	time++;
}

void Particle::Create(const Vector3& startPos, const Vector3& size, const Vector3& startSpeed)
{
	pos = startPos;
	speed = startSpeed;
	this->size = size;
	time = 0;
	isAlive = true;
}

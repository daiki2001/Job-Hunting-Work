#include "BombParticle.h"
#include <random>
#include "./Header/Parameter.h"
#include "./Header/Error.h"

const int BombParticle::MAX_TIME = 20;
DrawPolygon* BombParticle::draw = nullptr;
int BombParticle::model = FUNCTION_ERROR;

BombParticle::BombParticle() :
	particle{}
{
}

BombParticle::~BombParticle()
{
}

void BombParticle::Init(DrawPolygon* draw)
{
	BombParticle::draw = draw;
	model = draw->CreateSphere(0.1f, 4);
}

void BombParticle::Create(const Vector3& startPos)
{
	static std::mt19937 randEngine{ std::random_device{}()};
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
	static const size_t CREATE_NUM = 500;

	for (size_t i = 0, j = 0; i < CREATE_NUM; j++)
	{
		if (j > particle.size())
		{
			if (particle[j].GetAlive()) continue;
		}
		else
		{
			Particle createParticle;
			particle.push_back(createParticle);
		}

		Vector3 speed = Vector3::Zero();
		float randResult = dist(randEngine);
		speed.x = randResult;
		randResult = dist(randEngine);
		speed.y = randResult;
		randResult = dist(randEngine);
		speed.z = randResult;
		particle[j].Create(startPos, speed.Normalize() * 0.25f);
		i++;
	}
}

void BombParticle::Update()
{
	for (auto& i : particle)
	{
		if (i.GetAlive() == false) continue;

		i.Update(MAX_TIME);
	}
}

void BombParticle::Draw(const Vector3& offset)
{
	draw->ChangeOBJShader();

	for (auto& i : particle)
	{
		if (i.GetAlive() == false) continue;

		float particleTime = static_cast<float>(MAX_TIME - i.GetTime()) / static_cast<float>(MAX_TIME);
		draw->Draw(
			model,
			i.GetPos(),
			Math::Identity(),
			i.GetSize() * particleTime,
			DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, particleTime),
			Parameter::Get("white1x1"));
	}
}

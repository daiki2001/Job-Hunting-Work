#include "BombParticle.h"
#include "./Header/Parameter.h"
#include "./Header/Error.h"

DrawPolygon* BombParticle::draw = nullptr;
int BombParticle::model = FUNCTION_ERROR;
std::random_device BombParticle::seed_gen{};
std::mt19937 BombParticle::randEngine{};

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
	model = draw->CreateSphere(0.1f, 3);
}

void BombParticle::Create(const Vector3& startPos)
{
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
		float randResult = static_cast<float>(randEngine() / 1000);
		speed.x = randResult;
		randResult = static_cast<float>(randEngine() / 1000);
		speed.y = randResult;
		particle[j].Create(startPos, speed.Normalize() * 5.0f);
		i++;
	}
}

void BombParticle::Update()
{
	for (auto& i : particle)
	{
		if (i.GetAlive() == false) continue;

		i.Update(10);
	}
}

void BombParticle::Draw(const Vector3& offset)
{
	draw->ChangeOBJShader();

	for (auto& i : particle)
	{
		if (i.GetAlive() == false) continue;

		draw->Draw(model, i.GetPos(), Math::Identity(), i.GetSize(), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), Parameter::Get("white1x1"));
	}
}

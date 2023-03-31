#include "FireParticle.h"
#include "./Header/Parameter.h"

const int FireParticle::MAX_TIME = 10;
int FireParticle::model = FUNCTION_ERROR;

FireParticle::FireParticle() :
	particle{}
{
}

FireParticle::~FireParticle()
{
}

void FireParticle::Init()
{
	model = Particle::GetDraw()->CreateSphere(0.1f, 4);
}

void FireParticle::Create(const Vector3& startPos)
{
	static const size_t CREATE_NUM = 1;

	for (size_t i = 0, j = 0; i < CREATE_NUM; j++)
	{
		if (j < particle.size())
		{
			if (particle[j].GetAlive()) continue;
		}
		else
		{
			Particle createParticle;
			particle.push_back(createParticle);
		}

		const float speedLength = 0.125f;
		Vector3 speed = Vector3(0.0f, 1.0f, 0.0f);
		speed = { RandomNumber(1.0f, -1.0f), RandomNumber(1.0f, -1.0f), -1.0f };
		particle[j].Create(startPos, speed.Normalize() * speedLength, -speed.Normalize() * (speedLength / 10.0f));
		i++;
	}
}

void FireParticle::Update()
{
	for (auto& i : particle)
	{
		if (i.GetAlive() == false) continue;

		i.Update(MAX_TIME);
	}
}

void FireParticle::Draw(const Vector3& offset)
{
	auto draw = Particle::GetDraw();
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
			Color::AddAlphaValue(Color::RED, particleTime),
			Parameter::Get("white1x1"));
	}
}

#include "AppearParticle.h"
#include "./Header/DrawPolygon.h"
#include "./Header/Parameter.h"

const int AppearParticle::MAX_TIME = 10;
int AppearParticle::model = FUNCTION_ERROR;

AppearParticle::AppearParticle() :
	particle{}
{
}

AppearParticle::~AppearParticle()
{
}

void AppearParticle::Init()
{
	model = Library::DrawPolygon::GetInstance()->CreateSphere(0.1f, 6);
}

void AppearParticle::Create(const Vector3& startPos)
{
	static const size_t CREATE_NUM = 20;
	Vector3 speed = Vector3(0.0f, 0.0f, -1.0f);

	for (size_t i = 0, j = 0; i < CREATE_NUM; j++)
	{
		Vector3 scale = Vector3::Scale_xyz(1.0f);
		scale *= RandomNumber(1.75f, 1.0f);

		if (j < particle.size())
		{
			if (particle[j].GetAlive()) continue;
		}
		else
		{
			Particle createParticle;
			particle.push_back(createParticle);
		}

		float speedLength = 0.125f;
		float accelLength = 1.0f / 32.0f;
		Vector3 accel = { RandomNumber(1.0f, -1.0f), RandomNumber(1.0f, -1.0f), 0.0f };
		particle[j].Create(startPos, speed.Normalize() * speedLength, accel.Normalize() * accelLength, scale);
		i++;
	}
}

void AppearParticle::Update()
{
	for (auto& i : particle)
	{
		if (i.GetAlive() == false) continue;

		i.Update(MAX_TIME);
	}
}

void AppearParticle::Draw(const Vector3& offset)
{
	auto draw = Library::DrawPolygon::GetInstance();
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
			Color::AddAlphaValue(Color::YELLOW, 1.0f),
			Parameter::Get("white1x1"));
	}
}

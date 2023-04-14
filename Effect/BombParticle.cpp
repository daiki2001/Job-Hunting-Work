#include "BombParticle.h"
#include "./Header/DrawPolygon.h"
#include "./Header/Parameter.h"

const int BombParticle::MAX_TIME = 20;
int BombParticle::model = FUNCTION_ERROR;

BombParticle::BombParticle() :
	particle{}
{
}

BombParticle::~BombParticle()
{
}

void BombParticle::Init()
{
	model = Library::DrawPolygon::GetInstance()->CreateSphere(0.1f, 6);
}

void BombParticle::Create(const Vector3& startPos)
{
	static const size_t CREATE_NUM = 500;

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

		Vector3 speed = Vector3::Zero();
		speed = { RandomNumber(1.0f, -1.0f), RandomNumber(1.0f, -1.0f), RandomNumber(1.0f, -1.0f) };
		float speedLength = RandomNumber(0.5f, 0.1f);
		particle[j].Create(startPos, speed.Normalize() * speedLength, Vector3::Zero(), scale);
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
			Color::AddAlphaValue(Color::RED, particleTime),
			Parameter::Get("white1x1"));
	}
}

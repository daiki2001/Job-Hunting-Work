#pragma once
#include "./Header/EngineGeneral.h"

class Particle
{
private: //エイリアス
	using Vector3 = Math::Vector3;

private:
	Vector3 pos;
	Vector3 speed;
	Vector3 size;
	int time;
	bool isAlive;

public:
	Particle();
	~Particle() = default;

	// 初期化処理
	void Init();
	// 更新処理
	void Update(const int& maxTime);
	// 生成処理
	void Create(const Vector3& startPos, const Vector3& size, const Vector3& startSpeed = {});

	Vector3 GetPos() const { return pos; }
	Vector3 GetSpeed() const { return speed; }
	Vector3 GetSize() const { return size; }
	int GetTime() const { return time; }
	bool GetAlive() const { return isAlive; }
};
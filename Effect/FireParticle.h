#pragma once
#include "Particle.h"
#include <vector>
#include "./Header/EngineGeneral.h"

class FireParticle
{
private: //エイリアス
	using Vector3 = Math::Vector3;

public: //定数
	static const int MAX_TIME;

private: //静的メンバ変数
	static int model;

public: //静的メンバ関数
	// 初期化処理
	static void Init();

private: //メンバ変数
	std::vector<Particle> particle;

public: //メンバ関数
	FireParticle();
	~FireParticle();

	// 生成処理
	void Create(const Vector3& startPos);
	// 更新処理
	void Update();
	// 描画処理
	void Draw(const Vector3& offset = Vector3::Zero());
};

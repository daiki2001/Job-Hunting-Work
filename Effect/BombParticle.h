#pragma once
#include "Particle.h"
#include <vector>
#include <random>
#include "./Header/DrawPolygon.h"

class BombParticle
{
private: //エイリアス
	using Vector3 = Math::Vector3;
	
private: //静的メンバ変数
	static DrawPolygon* draw;
	static int model;
	static std::random_device seed_gen;
	static std::mt19937 randEngine;
	
public: //静的メンバ関数
	// 初期化処理
	static void Init(DrawPolygon* draw);
	
private: //メンバ変数
	std::vector<Particle> particle;

public: //メンバ関数
	BombParticle();
	~BombParticle();

	// 生成処理
	void Create(const Vector3& startPos);
	// 更新処理
	void Update();
	// 描画処理
	void Draw(const Vector3& offset = Vector3::Zero());
};
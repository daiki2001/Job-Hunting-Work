#pragma once
#include "Item.h"
#include "../Effect/BombParticle.h"
#include "./Header/EngineGeneral.h"

class Bomb : public Item
{
public: //エイリアス
	using Vector3 = Math::Vector3;

private: //静的メンバ変数
	BombParticle effect;
	
private: //メンバ変数
	Vector3 pos;          //座標
	unsigned int maxTime; //起爆までの時間
	unsigned int nowTime; //経過時間
	bool isBomber;        //爆発しているかどうか
	bool isAlive;         //置かれているかどうか

public: //メンバ関数
	Bomb(unsigned int maxCount, unsigned int time);
	~Bomb() override {}

	// 初期化処理
	void Init();
	// 更新処理
	void Update();
	// 描画処理
	void Draw(int offsetX = 0, int offsetY = 0) override;

	// 爆弾の設置
	void Set(Vector3 pos);

	const Vector3 GetPos() const { return pos; }
	const bool GetAlive() const { return isAlive; }
};

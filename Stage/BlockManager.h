#pragma once
#include "BlockType.h"
#include "../Player/Player.h"
#include <vector>
#include "./Header/EngineGeneral.h"
#include <map>

class BlockManager
{
public: //エイリアス
	template<class T> using vector = std::vector<T>;
	using Vector3 = Math::Vector3;
	using Ease = Math::Ease;

public: //サブクラス
	enum TypeId
	{
		NONE,             //ブロックなし
		WALL,             //壁ブロック
		GOAL,             //ゴール
		SWITCH,           //スイッチ
		KEY,              //鍵
		BOMB,             //爆弾
		MOVE_BLOCK,       //動かせるブロック
		SWITCH_BLOCK,     //スイッチがONの時に反応するブロック
		NOT_SWITCH_BLOCK, //スイッチがOFFの時に反応するブロック
		HOLE,             //穴
		UP_STAIRS,        //上り階段
		DOWN_STAIRS,      //下り階段
		MAX               //TypeIdの上限(プログラム用)
	};

	enum Step
	{
		STAY, //その階層にとどまる
		UP,   //上がる
		DOWN  //下がる
	};

	class Block
	{
	public: //メンバ変数
		Vector3 pos; //座標
		TypeId typeId;
		Ease ease;

	public: //メンバ関数
		Block(TypeId typeId);
		~Block() = default;
	};

private: //静的メンバ変数
	static Player* player;
	static bool isBlockSwitch;
	
	static Ease switchEase;
	static Ease notSwitchEase;

public: //静的メンバ関数
	static void EaseInit(vector<Block>& blocks);
	static void EaseUpdate(Block* blocks);

	static bool GetBlockSwitch() { return isBlockSwitch; }

private: //メンバ変数
	vector<BlockType> blockType;
	vector<Block> blocks;
	bool isSwitch;
	bool isGoal;
	std::map<int, TypeId> initPos; //ブロックの初期化
	Step step;
	int playerInitPos; //プレイヤーの初期位置(マス目)
	bool isPlayerMove;

public: //メンバ関数
	BlockManager();
	~BlockManager();

	// 初期化処理
	void Init(DrawPolygon* const draw);
	// 更新処理
	void Update();
	// 描画処理
	void Draw(const Vector3& offset = Vector3::Zero());
	// リセット処理
	void Reset();
	// マップのリセット処理
	void MapInit();

	// ブロックの生成処理
	int CreateBlock(TypeId typeId);
	// ブロックの切り替え処理
	void ChengeBlock(int index, TypeId typeId);
	// ブロックの削除処理
	void DeleteBlock(int index);
	// 全ブロックの削除処理
	void AllDeleteBlock();

	void MoveArea();

	Block& GetBlock(const size_t& index)
	{
		return blocks[index];
	};
	const size_t GetBlockSize() const { return blocks.size(); }
	const bool GetDoor() const { return isSwitch; }
	const bool GetGoal() const { return isGoal; }
	const int GetStep() const { return static_cast<int>(step); }

	/// <summary>
	/// プレイヤーの周囲のブロックを取得する
	/// </summary>
	/// <param name="radius"> 0を指定するとプレイヤーがいる場所のブロックだけ渡す </param>
	/// <param name="surroundingBlockType"> 要素番号をまとめた配列 </param>
	/// <returns> プレイヤーが踏んでいるブロックの要素番号 </returns>
	int GetSurroundingBlock(int radius, TypeId* surroundingBlockType) const;

	bool IsPlayerMove() { return isPlayerMove; }
private:
	// プレイヤーの押し戻し処理
	void PlayerPushBack(int index) const;
	// スイッチが押された時の処理
	void SwitchPush();
	// 押せるブロックの処理
	void PushBlock(int index);
};

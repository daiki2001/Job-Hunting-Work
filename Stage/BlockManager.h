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

public: //サブクラス
	enum TypeId
	{
		NONE,   //ブロックなし
		WALL,   //壁ブロック
		GOAL,   //ゴール
		SWITCH, //スイッチ
		KEY,    //鍵
		BOMB,   //爆弾
		MAX     //TypeIdの上限(プログラム用)
	};

	class Block
	{
	public: //メンバ変数
		Vector3 pos; //座標
		bool isSwitch;
		TypeId typeId;

	public: //メンバ関数
		Block(TypeId typeId);
		~Block() = default;
	};

private: //静的メンバ変数
	static Player* player;

private: //メンバ変数
	vector<BlockType> blockType;
	vector<Block> blocks;
	bool isOpen;
	bool isGoal;
	std::map<int, TypeId> itemInitPos;

public: //メンバ関数
	BlockManager();
	~BlockManager();

	// 初期化処理
	void Init(DrawPolygon* const draw);
	// 更新処理
	void Update();
	// 描画処理
	void Draw(int offsetX = 0, int offsetY = 0);
	// リセット処理
	void Reset();

	// ブロックの生成処理
	int CreateBlock(TypeId typeId);
	// ブロックの切り替え処理
	void ChengeBlock(int index, TypeId typeId);
	// ブロックの削除処理
	void DeleteBlock(int index);
	// 全ブロックの削除処理
	void AllDeleteBlock();

	Block& GetBlock(int index)
	{
		return blocks[index];
	};
	const size_t GetBlockSize() const { return blocks.size(); }
	const bool GetDoor() const { return isOpen; }
	const bool GetGoal() const { return isGoal; }

	/// <summary>
	/// プレイヤーの周囲のブロックを取得する
	/// </summary>
	/// <param name="radius"> 0を指定するとプレイヤーがいる場所のブロックだけ渡す </param>
	/// <param name="surroundingBlockType"> 要素番号をまとめた配列 </param>
	/// <returns> プレイヤーが踏んでいるブロックの要素番号 </returns>
	int GetSurroundingBlock(int radius, TypeId* surroundingBlockType) const;
private:
	// プレイヤーの押し戻し処理
	void PlayerPushBack(int index) const;
	// スイッチが押された時の処理
	void SwitchPush(const size_t& blockNo);
};

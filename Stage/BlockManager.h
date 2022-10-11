#pragma once
#include "BlockType.h"
#include "../Player.h"
#include <vector>
#include "./Header/EngineGeneral.h"

class BlockManager
{
public: //エイリアス
	template<class T> using vector = std::vector<T>;
	using Vector3 = Math::Vector3;

public: //サブクラス
	enum TypeId
	{
		NONE,
		WALL,
		GOAL,
		SWITCH,
		KEY,
		MAX
	};

	class Block
	{
	public: //メンバ変数
		Vector3 pos; //座標
		bool isSwitch;
		TypeId typeId;

	public: //メンバ関数
		Block(const TypeId& typeId);
		~Block() = default;
	};

private: //静的メンバ変数
	static Player* player;

private: //メンバ変数
	vector<BlockType> blockType;
	vector<Block> blocks;
	bool isOpen;
	bool isGoal;
	vector<int> keyInitPos;

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
	int CreateBlock(const TypeId& typeId);
	// ブロックの切り替え処理
	void ChengeBlock(int index, const TypeId& typeId);
	// ブロックの削除処理
	void DeleteBlock(int index);
	// 全ブロックの削除処理
	void AllDeleteBlock();

	Block& GetBlock(int index)
	{
		return blocks[index];
	};
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
	void PlayerPushBack() const;
	// スイッチが押された時の処理
	void SwitchPush(const size_t& blockNo);
};

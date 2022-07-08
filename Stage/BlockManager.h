#pragma once
#include "BlockType.h"
#include <vector>

class BlockManager final
{
public: //シングルトン化
	static BlockManager* Get();
private:
	BlockManager();
	BlockManager(const BlockManager&) = delete;
	~BlockManager();
	BlockManager operator=(const BlockManager&) = delete;

public: //サブクラス
	enum TypeId
	{
		NONE,
		WALL
	};

	class Block
	{
	public: //メンバ変数
		int posX = 0;
		int posY = 0;
	private:
		TypeId typeId = BlockManager::TypeId::NONE;

	public: //メンバ関数
		Block() = default;
		~Block() = default;

		const TypeId GetTypeId() const { return typeId; }
		void SetTypeId(const TypeId& typeId) { this->typeId = typeId; }
	};

private: //メンバ変数
	std::vector<BlockType> blockType;
	std::vector<Block> block;

public: //メンバ関数
	// 初期化処理
	void Init(DrawPolygon* const draw);
	// 更新処理
	void Update();
	// 描画処理
	void Draw(const int& offsetX = 0, const int& offsetY = 0);

	// ブロックの生成処理
	int CreateBlock(const BlockManager::TypeId& typeId);

	Block& GetBlock(const int& index) { return block[index]; };
private:
	/// <summary>
	/// プレイヤーの周囲のブロックを取得する
	/// </summary>
	/// <param name="radius"> 0を指定するとプレイヤーがいる場所のブロックだけ渡す </param>
	/// <param name="surroundingBlockType"> 要素番号をまとめた配列 </param>
	void GetSurroundingBlock(const int& radius, BlockManager::TypeId* surroundingBlockType);
};

#pragma once
#include "BlockType.h"
#include <vector>
#include "./Header/EngineGeneral.h"

class BlockManager final
{
public: //シングルトン化
	static BlockManager* Get();
private:
	BlockManager();
	BlockManager(const BlockManager&) = delete;
	~BlockManager();
	BlockManager operator=(const BlockManager&) = delete;

public: //エイリアス
	using Vector3 = Math::Vector3;

public: //サブクラス
	enum TypeId
	{
		NONE,
		WALL,
		SWITCH
	};

	class Block
	{
	public: //メンバ変数
		Vector3 pos; //座標
		bool isSwitch;
	private:
		TypeId typeId;

	public: //メンバ関数
		Block(const TypeId& typeId);
		~Block() = default;

		const TypeId GetTypeId() const { return typeId; }
	};

private: //メンバ変数
	std::vector<BlockType> blockType;
	std::vector<Block> block;
	bool isOpen;

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
	const bool GetDoor() const { return isOpen; }
private:
	// プレイヤーの押し戻し処理
	void PlayerPushBack() const;
	// スイッチが押された時の処理
	void SwitchPush(const size_t& blockNo);

	/// <summary>
	/// プレイヤーの周囲のブロックを取得する
	/// </summary>
	/// <param name="radius"> 0を指定するとプレイヤーがいる場所のブロックだけ渡す </param>
	/// <param name="surroundingBlockType"> 要素番号をまとめた配列 </param>
	/// <returns> プレイヤーが踏んでいるブロックの要素番号 </returns>
	int GetSurroundingBlock(const int& radius, BlockManager::TypeId* surroundingBlockType);
};

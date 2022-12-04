#pragma once
#include "BaseUndoRedo.h"
#include "./Header/EngineGeneral.h"
#include "../Stage/BlockManager.h"

class AddBlock final : public BaseUndoRedo
{
public: //メンバ変数
	Math::Vector3 roomPos;
	int mapIndex;
	BlockManager::TypeId blockIndex;
	BlockManager::TypeId oldBlockIndex;

public: //メンバ関数
	AddBlock(const Math::Vector3& roomPos, int mapIndex, BlockManager::TypeId blockIndex,
			 BlockManager::TypeId oldBlockIndex);
	~AddBlock() override = default;

	void Undo() override;
	void Redo() override;
private:
	AddBlock() = delete;
};

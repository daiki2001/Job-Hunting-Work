#include "AddBlock.h"
#include "./Stage/Stage.h"

AddBlock::AddBlock(const Math::Vector3& roomPos, int mapIndex, BlockManager::TypeId blockIndex,
				   BlockManager::TypeId oldBlockIndex) :
	BaseUndoRedo(),
	roomPos(roomPos),
	mapIndex(mapIndex),
	blockIndex(blockIndex),
	oldBlockIndex(oldBlockIndex)
{
}

void AddBlock::Undo()
{
	if (Stage::SetRoom(roomPos) == FUNCTION_ERROR) return;
	Stage::GetBlockManager()->ChengeBlock(mapIndex, oldBlockIndex);
}

void AddBlock::Redo()
{
	if (Stage::SetRoom(roomPos) == FUNCTION_ERROR) return;
	Stage::GetBlockManager()->ChengeBlock(mapIndex, blockIndex);
}

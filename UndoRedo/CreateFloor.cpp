#include "CreateFloor.h"
#include "./Stage/Stage.h"

CreateFloor::CreateFloor(int mapIndex, const Math::Vector3& roomPos, const Math::Vector3& oldRoomPos, bool isUp) :
	BaseUndoRedo(),
	mapIndex(mapIndex),
	roomPos(roomPos),
	oldRoomPos(oldRoomPos),
	isUp(isUp)
{
}

void CreateFloor::Undo()
{
	if (Stage::SetRoom(roomPos) == FUNCTION_ERROR) return;
	Stage::GetBlockManager()->ChengeBlock(mapIndex, BlockManager::TypeId::NONE);
	Stage::SetRoom(oldRoomPos);
	Stage::GetBlockManager()->ChengeBlock(mapIndex, BlockManager::TypeId::NONE);
}

void CreateFloor::Redo()
{
	if (Stage::SetRoom(roomPos) == FUNCTION_ERROR) return;

	if (isUp)
	{
		Stage::GetBlockManager()->ChengeBlock(mapIndex, BlockManager::TypeId::UP_STAIRS);
		Stage::SetRoom(roomPos);
		Stage::GetBlockManager()->ChengeBlock(mapIndex, BlockManager::TypeId::DOWN_STAIRS);
	}
	else
	{
		Stage::GetBlockManager()->ChengeBlock(mapIndex, BlockManager::TypeId::DOWN_STAIRS);
		Stage::SetRoom(roomPos);
		Stage::GetBlockManager()->ChengeBlock(mapIndex, BlockManager::TypeId::UP_STAIRS);
	}
}

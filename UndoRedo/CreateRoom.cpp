#include "CreateRoom.h"
#include "./Stage/Stage.h"

CreateRoom::CreateRoom(const Math::Vector3& roomPos, const Math::Vector3& oldRoomPos) :
	BaseUndoRedo(),
	roomPos(roomPos),
	oldRoomPos(oldRoomPos)
{
}

void CreateRoom::Undo()
{
	Stage::Get()->GetArea(roomPos).isAlive = false;
	Stage::SetRoom(oldRoomPos);
}

void CreateRoom::Redo()
{
	Stage::Get()->GetArea(roomPos).isAlive = true;
	Stage::SetRoom(roomPos);
}

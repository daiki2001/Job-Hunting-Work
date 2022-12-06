#include "AddDoor.h"
#include "./Stage/Stage.h"

AddDoor::AddDoor(const Math::Vector3& roomPos, Area::DoorNum doorNum,
				 Door::DoorStatus doorIndex, Door::DoorStatus oldDoorIndex, int cursorState) :
	BaseUndoRedo(),
	roomPos(roomPos),
	doorNum(doorNum),
	doorIndex(doorIndex),
	oldDoorIndex(oldDoorIndex),
	cursorState(cursorState)
{
}

void AddDoor::Undo()
{
	if (Stage::SetRoom(roomPos) == FUNCTION_ERROR) return;
	Stage::SetDoorStatus(oldDoorIndex, doorNum);
}

void AddDoor::Redo()
{
	if (Stage::SetRoom(roomPos) == FUNCTION_ERROR) return;
	Stage::SetDoorStatus(doorIndex, doorNum);
}

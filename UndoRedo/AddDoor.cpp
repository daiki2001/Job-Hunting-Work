#include "AddDoor.h"
#include "./Stage/Stage.h"

AddDoor::AddDoor(const Math::Vector3& roomPos, Area::DoorNum cursorState,
				 Door::DoorStatus doorIndex, Door::DoorStatus oldDoorIndex) :
	BaseUndoRedo(),
	roomPos(roomPos),
	cursorState(cursorState),
	doorIndex(doorIndex),
	oldDoorIndex(oldDoorIndex)
{
}

void AddDoor::Undo()
{
	if (Stage::SetRoom(roomPos) == FUNCTION_ERROR) return;
	Stage::SetDoorStatus(oldDoorIndex, cursorState);
}

void AddDoor::Redo()
{
	if (Stage::SetRoom(roomPos) == FUNCTION_ERROR) return;
	Stage::SetDoorStatus(doorIndex, cursorState);
}

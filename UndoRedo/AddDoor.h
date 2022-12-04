#pragma once
#include "BaseUndoRedo.h"
#include "./Header/EngineGeneral.h"
#include "../Stage/Area.h"

class AddDoor final : public BaseUndoRedo
{
public: //メンバ変数
	Math::Vector3 roomPos;
	Area::DoorNum cursorState;
	Door::DoorStatus doorIndex;
	Door::DoorStatus oldDoorIndex;

public: //メンバ関数
	AddDoor(const Math::Vector3& roomPos, Area::DoorNum cursorState,
			Door::DoorStatus doorIndex, Door::DoorStatus oldDoorIndex);
	~AddDoor() override = default;

	void Undo() override;
	void Redo() override;
private:
	AddDoor() = delete;
};

#pragma once
#include "BaseUndoRedo.h"
#include "./Header/EngineGeneral.h"
#include "../Stage/Area.h"

class AddDoor final : public BaseUndoRedo
{
public: //メンバ変数
	Math::Vector3 roomPos;
	Area::DoorNum doorNum;
	Door::DoorStatus doorIndex;
	Door::DoorStatus oldDoorIndex;
	int cursorState;

public: //メンバ関数
	AddDoor(const Math::Vector3& roomPos, Area::DoorNum doorNum,
			Door::DoorStatus doorIndex, Door::DoorStatus oldDoorIndex,
			int cursorState);
	~AddDoor() override = default;

	void Undo() override;
	void Redo() override;
private:
	AddDoor() = delete;
};

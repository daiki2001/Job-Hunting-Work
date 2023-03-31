#pragma once
#include "BaseUndoRedo.h"
#include "./Header/EngineGeneral.h"

class CreateFloor final : public BaseUndoRedo
{
public: //メンバ変数
	int mapIndex;
	Math::Vector3 roomPos;
	Math::Vector3 oldRoomPos;
	bool isUp;

public: //メンバ関数
	CreateFloor(int mapIndex, const Math::Vector3& roomPos, const Math::Vector3& oldRoomPos, bool isUp);
	~CreateFloor() override = default;

	void Undo() override;
	void Redo() override;
private:
	CreateFloor() = delete;
};

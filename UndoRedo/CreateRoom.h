#pragma once
#include "BaseUndoRedo.h"
#include "./Header/EngineGeneral.h"

class CreateRoom final : public BaseUndoRedo
{
public: //メンバ変数
	Math::Vector3 roomPos;
	Math::Vector3 oldRoomPos;

public: //メンバ関数
	CreateRoom(const Math::Vector3& roomPos, const Math::Vector3& oldRoomPos);
	~CreateRoom() override = default;

	void Undo() override;
	void Redo() override;
private:
	CreateRoom() = delete;
};

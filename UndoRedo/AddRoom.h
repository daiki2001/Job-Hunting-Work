#pragma once
#include "BaseUndoRedo.h"

class AddRoom final : public BaseUndoRedo
{
public: //メンバ変数
	int mapIndex;
	int blockIndex;

public: //メンバ関数
	AddRoom(int mapIndex, int blockIndex) :
		BaseUndoRedo(),
		mapIndex(mapIndex),
		blockIndex(blockIndex)
	{
	}
	~AddRoom() override = default;

	void Undo() override;
	void Redo() override;
private:
	AddRoom() = delete;
};

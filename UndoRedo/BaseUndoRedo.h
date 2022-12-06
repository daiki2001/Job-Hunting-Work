#pragma once

class BaseUndoRedo
{
public: //メンバ関数
	BaseUndoRedo() = default;
	virtual ~BaseUndoRedo() = default;

	virtual void Undo() {}
	virtual void Redo() {}
};

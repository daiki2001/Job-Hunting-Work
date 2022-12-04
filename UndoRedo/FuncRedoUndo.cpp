#include "FuncRedoUndo.h"
#include <algorithm>
#include "./Header/Error.h"

FuncRedoUndo::FuncRedoUndo() :
	commandList{},
	commandIndex(0)
{
}

FuncRedoUndo::~FuncRedoUndo()
{
	for (auto itr = commandList.begin(); itr != commandList.end();)
	{
		itr = commandList.erase(itr);
	}
}

int FuncRedoUndo::UndoCommand()
{
	if (commandList.size() == 0)
	{
		return Engine::FUNCTION_ERROR;
	}

	auto itr = commandList.begin();
	for (int i = 0; i < commandIndex; i++)
	{
		itr++;
	}

	itr->undoRedo->Undo();
	if (commandIndex >= 0)
	{
		commandIndex--;
	}
	return 0;
}

int FuncRedoUndo::RedoCommand()
{
	if (commandList.size() == 0 ||
		static_cast<size_t>(commandIndex + 1) >= commandList.size())
	{
		return Engine::FUNCTION_ERROR;
	}

	commandIndex++;
	auto itr = commandList.begin();
	for (int i = 0; i < commandIndex; i++)
	{
		itr++;
	}

	itr->undoRedo->Redo();
	return 0;
}

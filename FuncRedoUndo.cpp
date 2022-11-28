#include "FuncRedoUndo.h"
#include "./Header/Error.h"

FuncRedoUndo::FuncRedoUndo() :
	commandList{},
	commandIndex(0)
{
}

int FuncRedoUndo::AddCommandList(const std::string& name, pFuncType func)
{
	for (auto& i : commandList)
	{
		if (name == i.name)
		{
			OutputDebugStringA("同名のコマンドが見つかりました。\n");
			return Engine::FUNCTION_ERROR;
		}
	}

	commandList.push_back({ name, func });
	return static_cast<int>(commandList.size() - 1U);
}

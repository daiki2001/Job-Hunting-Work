#pragma once
#include <list>
#include <string>
#include <memory>
#include "BaseUndoRedo.h"

class FuncRedoUndo
{
public: //サブクラス
	struct Command
	{
	public:
		std::string name;
		std::shared_ptr<BaseUndoRedo> undoRedo;
	};

private: //メンバ変数
	std::list<Command> commandList;
	int commandIndex;

public: //メンバ関数
	FuncRedoUndo();
	~FuncRedoUndo();

	// 元に戻す
	int UndoCommand();
	// やり直す
	int RedoCommand();
	// コマンドの追加
	template <class T>
	void AddCommandList(const std::string& name, const T& undoRedo)
	{
		if (commandList.size() != 0)
		{
			auto itr = commandList.begin();
			for (int i = 0; i <= commandIndex; i++)
			{
				itr++;
			}
			for (; itr != commandList.end();)
			{
				itr = commandList.erase(itr);
			}
		}

		commandList.push_back({ name, std::make_shared<T>(undoRedo) });
		commandIndex = static_cast<int>(commandList.size() - 1);
		commandList.back().undoRedo.get()->Redo();
	}
};
#pragma once
#include <list>
#include <string>
#include <vector>

// 関数ポインタ用の型名定義
using pFuncType = void (*)(void);

class FuncRedoUndo
{
public: //サブクラス
	struct Command
	{
	public:
		std::string name;
		pFuncType commandFunc;
	};
	
private: //メンバ変数
	std::vector<Command> commandList;
	size_t commandIndex;

public: //メンバ関数
	FuncRedoUndo();
	~FuncRedoUndo();

	// コマンドの追加
	int AddCommandList(const std::string& name, pFuncType func);
};
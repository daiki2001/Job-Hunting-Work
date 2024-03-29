﻿#pragma once
#include <string>

class LoadGraph final
{
public: //シングルトン化
	static LoadGraph* Get();
private:
	LoadGraph();
	LoadGraph(const LoadGraph&) = delete;
	LoadGraph operator=(const LoadGraph&) = delete;
	~LoadGraph();

private: //エイリアス
	using string = std::string;

public: //定数
	static const string WALL_BLOCK;
	static const string GOAL;
	static const string SWITCH_UI;
	static const string KEY;
	static const string BOMB;
	static const string ARROW;
	static const string UP_STAIRS;
	static const string DOWN_STAIRS;
	static const string WHITE_TILE;
	static const string TORCH;
	static const string TRIANGLE;
	static const string BRACELET;

	static const string WALL_UI;
	static const string KEY_CLOSE;
	static const string BREAK_WALL_UI;
	static const string CREATE_ROOM;
	
	static const string KEYBOARD;
	static const string PUSH_KEYBOARD;
	static const string CURSOR;

private: //メンバ変数
	bool isLoad;

public: //メンバ関数
	// 読み込み処理
	void Load();
	// 解放処理
	void Release() {}
};

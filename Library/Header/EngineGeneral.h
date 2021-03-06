#pragma once
#include "./Math/EngineMath.h"
#include "LoadFile.h"
#include "Error.h"

/*定数*/
namespace Engine
{
static const std::string libraryDirectory = "./lib/";             //ライブラリディレクトリ
static const std::string defaultTextureFileName = "white1x1.png"; //デフォルト画像
} //Engine

/*関数*/
namespace Engine
{
// ディレクトリを含んだファイルパスからディレクトリパスを抽出する
std::string ExtractDirectory(const std::string& path);
// ディレクトリを含んだファイルパスからファイル名を抽出する
std::string ExtractFileName(const std::string& path);

template <class T>
constexpr void Swap(T* a, T* b)
{
	T tmp = *a;
	*a = *b;
	*b = tmp;
}
} //Engine

using namespace Engine;

#pragma once
#include <string>
#include <vector>

/*関数*/
namespace Engine
{
namespace Load
{
/// <summary>
/// マップチップのファイル読み込み
/// </summary>
/// <param name="mapArray"> 読み込んだマップチップの保存先 </param>
/// <param name="mapSize"> mapArrayのサイズ </param>
/// <param name="FilePath"> CSVファイルのパス </param>
/// <param name="LoadStopNumber"> ここに指定された値がヒットすると読み込みを終了する </param>
/// <returns> 0で成功、0以外で失敗 </returns>
int LoadMapChip(int* mapArray, const size_t& mapSize, const char* FilePath, int LoadStopNumber = -1);
} //Load
} //Engine

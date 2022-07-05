#pragma once
#include "./Header/DrawPolygon.h"
#include <string>

#ifdef _DEBUG
#include <DirectXMath.h>
#endif // _DEBUG

class BlockType
{
public: //定数
	static const int WIDTH;
	static const int HEIGHT;
	static const std::wstring blockResourcesDir;

private: //メンバ変数
	DrawPolygon* const draw;

	int typeId;
	int graph;

public: //メンバ関数
	BlockType(const int& typeId, DrawPolygon* const draw);
	virtual ~BlockType();

	// 生成処理
	virtual int Create(const wchar_t* filename);
	// 描画処理
	void Draw(const int& posX, const int& posY);
#ifdef _DEBUG
	void Draw(const int& posX, const int& posY, const DirectX::XMFLOAT4& color);
#endif // _DEBUG

	// IDの取得
	const int GetId() const { return typeId; }
};

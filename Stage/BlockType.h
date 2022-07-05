#pragma once
#include "./Header/DrawPolygon.h"
#include <string>

#ifdef _DEBUG
#include <DirectXMath.h>
#endif // _DEBUG

class BlockType
{
public: //’è”
	static const int WIDTH;
	static const int HEIGHT;
	static const std::wstring blockResourcesDir;

private: //ƒƒ“ƒo•Ï”
	DrawPolygon* const draw;

	int typeId;
	int graph;

public: //ƒƒ“ƒoŠÖ”
	BlockType(const int& typeId, DrawPolygon* const draw);
	virtual ~BlockType();

	// ¶¬ˆ—
	virtual int Create(const wchar_t* filename);
	// •`‰æˆ—
	void Draw(const int& posX, const int& posY);
#ifdef _DEBUG
	void Draw(const int& posX, const int& posY, const DirectX::XMFLOAT4& color);
#endif // _DEBUG

	// ID‚Ìæ“¾
	const int GetId() const { return typeId; }
};

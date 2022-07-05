#pragma once
#include "./Header/DrawPolygon.h"
#include <string>

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

	// ID‚Ìæ“¾
	const int GetId() const { return typeId; }
};

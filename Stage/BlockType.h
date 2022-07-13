#pragma once
#include "./Header/DrawPolygon.h"
#include <string>
#include "./Header/EngineGeneral.h"

class BlockType
{
public: //エイリアス
	using Vector3 = Math::Vector3;

public: //定数
	static const int WIDTH;
	static const int HEIGHT;
	static const std::wstring blockResourcesDir;

private: //静的メンバ変数
	static DrawPolygon* draw;
	static int BlockBox;
	
private: //メンバ変数
	int typeId;
	int graph;

public: //メンバ関数
	BlockType(const int& typeId, DrawPolygon* const draw);
	virtual ~BlockType();

	// 生成処理
	virtual int Create(const wchar_t* filename);
	// 描画処理
	void Draw(const Vector3& pos);
#ifdef _DEBUG
	void Draw(const Vector3& pos, const DirectX::XMFLOAT4& color);
#endif // _DEBUG

	// IDの取得
	const int GetId() const { return typeId; }
};

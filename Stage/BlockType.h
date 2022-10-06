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
	static const std::string blockResourcesDir;

private: //静的メンバ変数
	static DrawPolygon* draw;
	
private: //メンバ変数
	int typeId;
	int graph;
	int blockBox;
	Math::Matrix4 rotation;
	Math::Vector3 scale;
	DirectX::XMFLOAT4 color;

public: //メンバ関数
	BlockType(const int& typeId, DrawPolygon* const draw);
	virtual ~BlockType();

	// 生成処理
	virtual int Create(const wchar_t* filename = nullptr, const Math::Matrix4& rotation = Math::Identity(),
					   const Math::Vector3& scale = { 1.0f, 1.0f, 1.0f }, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	virtual int Create(const char* filename, const Math::Matrix4& rotation = Math::Identity(),
					   const Math::Vector3& scale = { 1.0f, 1.0f, 1.0f }, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	// 描画処理
	void Draw(const Vector3& pos);
#ifdef _DEBUG
	void Draw(const Vector3& pos, const DirectX::XMFLOAT4& color);
#endif // _DEBUG

	// IDの取得
	const int GetId() const { return typeId; }
};

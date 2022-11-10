#pragma once
#include "./Header/DrawPolygon.h"
#include <string>
#include "./Header/EngineGeneral.h"

class BlockType
{
public: //エイリアス
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using Vector3 = Math::Vector3;
	using Matrix4 = Math::Matrix4;

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
	Matrix4 rotation;
	Vector3 scale;
	XMFLOAT4 color;

public: //メンバ関数
	BlockType(int typeId, DrawPolygon* const draw);
	virtual ~BlockType();

	// 生成処理
	virtual int Create(const wchar_t* filename = nullptr, const Matrix4& rotation = Math::Identity(),
					   const Vector3& scale = { 1.0f, 1.0f, 1.0f }, const XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	virtual int Create(const char* filename, const Matrix4& rotation = Math::Identity(),
					   const Vector3& scale = { 1.0f, 1.0f, 1.0f }, const XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	virtual int Create(int number, bool isObject, const Matrix4& rotation = Math::Identity(), const Vector3& scale = { 1.0f, 1.0f, 1.0f },
					   const XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	// 描画処理
	void Draw(const Vector3& pos);

	// IDの取得
	const int GetId() const { return typeId; }
};

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
	static const float BLOCK_SIZE;   //ブロックの大きさ
	static const float BLOCK_HEIGHT; //ブロックの高さ
	static const float FLOOR_HEIGHT; //床の厚さ
	static const std::string blockResourcesDir;

private: //静的メンバ変数
	static DrawPolygon* draw;
	
	static int floorGraph; //床のテクスチャ
	static int whiteTile;  //白の床タイルのテクスチャ
	static int floorObj;   //床のオブジェクト
	static int switchBlock;
	static int blueSwitchBlock;
	
public: //静的メンバ関数
	static void StaticInit(DrawPolygon* const draw);
	// 床の描画
	static void FloorDraw(const Vector3& offset = Vector3::Zero(), bool isWhiteTile = false);

	static DrawPolygon* GetDraw() { return draw; }

private: //メンバ変数
	int typeId;
	int graph;
	int blockBox;
	Matrix4 rotation;
	Vector3 scale;
	XMFLOAT4 color;

public: //メンバ関数
	BlockType(int typeId);
	virtual ~BlockType();

	// 生成処理
	int Create(const wchar_t* filename = nullptr, const Matrix4& rotation = Math::Identity(),
			   const Vector3& scale = { 1.0f, 1.0f, 1.0f }, const XMFLOAT4& color = Color::AddAlphaValue(Color::WHITE, 1.0f));
	int Create(const char* filename, const Matrix4& rotation = Math::Identity(),
			   const Vector3& scale = { 1.0f, 1.0f, 1.0f }, const XMFLOAT4& color = Color::AddAlphaValue(Color::WHITE, 1.0f));
	int Create(int number, bool isObject, const Matrix4& rotation = Math::Identity(), const Vector3& scale = { 1.0f, 1.0f, 1.0f },
					   const XMFLOAT4& color = Color::AddAlphaValue(Color::WHITE, 1.0f));
	// 描画処理
	void Draw(const Vector3& offset) const;

	// IDの取得
	const int GetId() const { return typeId; }
};

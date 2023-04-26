﻿#include "./Header/DrawPolygon.h"
#include "./Header/DirectXInit.h"
#include "./Header/Camera.h"
#include "./ShaderMgr/ShaderManager.h"
#include <fstream>
#include <sstream>
#include <string>

namespace
{
ShaderManager* shaderMgr = ShaderManager::Get();

static const VertexData defaultVertexData{};
}

namespace Engine
{
namespace Library
{
DrawPolygon::DrawPolygon() :
	DebugText(),
	polygonCount(0),
	loopCount(0),
	lightVec{},
	light(100, -100, 100),
	objModelCount(0),
	verticesCount{},
	material{}
{
}

DrawPolygon::~DrawPolygon()
{
	DataClear();
}

DrawPolygon* DrawPolygon::GetInstance()
{
	static DrawPolygon instance;
	return &instance;
}

int DrawPolygon::CreateTriangle(
	const Vector3& vertex1, const DirectX::XMFLOAT2& uv1,
	const Vector3& vertex2, const DirectX::XMFLOAT2& uv2,
	const Vector3& vertex3, const DirectX::XMFLOAT2& uv3,
	bool isFill)
{
	vertices.emplace_back(defaultVertexData);

	vertices.back().vertices.emplace_back(Vertex{ vertex1, {}, uv1 });
	vertices.back().vertices.emplace_back(Vertex{ vertex2, {}, uv2 });
	vertices.back().vertices.emplace_back(Vertex{ vertex3, {}, uv3 });

	if (isFill == true)
	{
		vertices.back().indices.emplace_back(0);
		vertices.back().indices.emplace_back(1);
		vertices.back().indices.emplace_back(2);
	}
	else
	{
		vertices.back().indices.emplace_back(0);
		vertices.back().indices.emplace_back(1);
		vertices.back().indices.emplace_back(1);
		vertices.back().indices.emplace_back(2);
		vertices.back().indices.emplace_back(2);
		vertices.back().indices.emplace_back(0);
	}

	return CreateVertexAndIndexBuffer();
}

int DrawPolygon::CreateRect(float width, float height, bool isFill)
{
	vertices.emplace_back(defaultVertexData);

	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, +height / 2, 0.0f }, {}, { 0.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, +height / 2, 0.0f }, {}, { 1.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, -height / 2, 0.0f }, {}, { 0.0f, 1.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, +height / 2, 0.0f }, {}, { 1.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, -height / 2, 0.0f }, {}, { 0.0f, 1.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, -height / 2, 0.0f }, {}, { 1.0f, 1.0f } });

	if (isFill == true)
	{
		vertices.back().indices.emplace_back(0);
		vertices.back().indices.emplace_back(1);
		vertices.back().indices.emplace_back(2);
		vertices.back().indices.emplace_back(4);
		vertices.back().indices.emplace_back(3);
		vertices.back().indices.emplace_back(5);
	}
	else
	{
		vertices.back().indices.emplace_back(0);
		vertices.back().indices.emplace_back(1);
		vertices.back().indices.emplace_back(3);
		vertices.back().indices.emplace_back(5);
		vertices.back().indices.emplace_back(5);
		vertices.back().indices.emplace_back(2);
		vertices.back().indices.emplace_back(4);
		vertices.back().indices.emplace_back(0);
	}

	return CreateVertexAndIndexBuffer();
}

int DrawPolygon::CreateCircle(float r, const size_t& divNum, bool isFill)
{
	vertices.emplace_back(defaultVertexData);

	Vertex* v = (Vertex*)malloc(sizeof(Vertex) * (divNum + 1));
	size_t indexSize = divNum * (2 + (size_t)isFill);
	unsigned short* index = (unsigned short*)malloc(sizeof(unsigned short) * indexSize);

	Circle({ 0, 0, 0 }, r, divNum, isFill, v, index);

	for (size_t i = 0; i < divNum + 1; i++)
	{
		vertices.back().vertices.emplace_back(v[i]);
	}
	for (size_t i = 0; i < indexSize; i++)
	{
		vertices.back().indices.emplace_back(index[i]);
	}

	free(v);
	free(index);

	for (size_t i = 0; i < vertices.back().vertices.size(); i++)
	{
		vertices.back().vertices[i].pos.x *= -1;
	}

	return CreateVertexAndIndexBuffer();
}

int DrawPolygon::Create3Dbox(float width, float height, float depth, bool isFill)
{
	vertices.emplace_back(defaultVertexData);

	// 前
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, +height / 2, -depth / 2 }, {}, { 0.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, +height / 2, -depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, +height / 2, -depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, -height / 2, -depth / 2 }, {}, { 1.0f, 1.0f } });
	// 後ろ
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, +height / 2, +depth / 2 }, {}, { 0.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, -height / 2, +depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, -height / 2, +depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, -height / 2, +depth / 2 }, {}, { 1.0f, 1.0f } });
	// 左
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, -height / 2, +depth / 2 }, {}, { 0.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, +height / 2, -depth / 2 }, {}, { 1.0f, 1.0f } });
	// 右
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, -height / 2, +depth / 2 }, {}, { 0.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, +height / 2, -depth / 2 }, {}, { 1.0f, 1.0f } });
	// 上
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, -height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, -height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, -height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, -height / 2, +depth / 2 }, {}, { 1.0f, 1.0f } });
	// 下
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, +height / 2, -depth / 2 }, {}, { 0.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, +height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, +height / 2, -depth / 2 }, {}, { 0.0f, 1.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { +width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 0.0f } });
	vertices.back().vertices.emplace_back(Vertex{ { -width / 2, +height / 2, +depth / 2 }, {}, { 1.0f, 1.0f } });

	if (isFill == true)
	{
		for (unsigned short i = 0; i < 6; i++)
		{
			vertices.back().indices.emplace_back(0 + 3 * 2 * i);
			vertices.back().indices.emplace_back(1 + 3 * 2 * i);
			vertices.back().indices.emplace_back(2 + 3 * 2 * i);
			vertices.back().indices.emplace_back(4 + 3 * 2 * i);
			vertices.back().indices.emplace_back(3 + 3 * 2 * i);
			vertices.back().indices.emplace_back(5 + 3 * 2 * i);
		}
	}
	else
	{
		for (unsigned short i = 0; i < 6; i++)
		{
			vertices.back().indices.emplace_back(0 + 2 * 4 * i);
			vertices.back().indices.emplace_back(1 + 2 * 4 * i);
			vertices.back().indices.emplace_back(3 + 2 * 4 * i);
			vertices.back().indices.emplace_back(5 + 2 * 4 * i);
			vertices.back().indices.emplace_back(5 + 2 * 4 * i);
			vertices.back().indices.emplace_back(2 + 2 * 4 * i);
			vertices.back().indices.emplace_back(4 + 2 * 4 * i);
			vertices.back().indices.emplace_back(0 + 2 * 4 * i);
		}
	}

	return CreateVertexAndIndexBuffer();
}

int DrawPolygon::CreateCorn(float r, float h, const size_t& divNum, bool isFill)
{
	vertices.emplace_back(defaultVertexData);

	Vertex* v = (Vertex*)malloc(sizeof(Vertex) * (divNum + 1));
	size_t indexSize = divNum * (2 + (size_t)isFill);
	unsigned short* index = (unsigned short*)malloc(sizeof(unsigned short) * indexSize);

	Circle({ 0, 0, 0 }, r, divNum, isFill, v, index);

	for (size_t i = 0; i < divNum + 1; i++)
	{
		vertices.back().vertices.emplace_back(v[i]);
	}
	for (size_t i = 0; i < indexSize; i++)
	{
		vertices.back().indices.emplace_back(index[i]);
	}

	free(v);
	free(index);

	size_t size = vertices.back().vertices.size();

	for (size_t i = 0; i < divNum; i++)
	{
		vertices.back().vertices.emplace_back(vertices.back().vertices[i + 0]);
		vertices.back().vertices.emplace_back(vertices.back().vertices[i + 1]);
		vertices.back().vertices.emplace_back(Vertex{
			{
				vertices.back().vertices[divNum].pos.x,
				vertices.back().vertices[divNum].pos.y,
				vertices.back().vertices[divNum].pos.z + h
			},
			vertices.back().vertices[divNum].normal,
			vertices.back().vertices[divNum].uv,
											  });

		if (isFill == true)
		{
			vertices.back().indices.emplace_back((unsigned short)(i * 3 + 0 + size));
			vertices.back().indices.emplace_back((unsigned short)(i * 3 + 1 + size));
			vertices.back().indices.emplace_back((unsigned short)(i * 3 + 2 + size));
		}
		else
		{
			vertices.back().indices.emplace_back((unsigned short)(i * 3 + 0 + size));
			vertices.back().indices.emplace_back((unsigned short)(i * 3 + 1 + size));
			vertices.back().indices.emplace_back((unsigned short)(i * 3 + 1 + size));
			vertices.back().indices.emplace_back((unsigned short)(i * 3 + 2 + size));
			vertices.back().indices.emplace_back((unsigned short)(i * 3 + 2 + size));
			vertices.back().indices.emplace_back((unsigned short)(i * 3 + 0 + size));
		}
	}

	vertices.back().vertices[divNum * 3 - 2 + size] =
		vertices.back().vertices[0];

	return CreateVertexAndIndexBuffer();
}

int DrawPolygon::CreateCylinder(float r, float h, const size_t& divNum, bool isFill)
{
	vertices.emplace_back(defaultVertexData);

	Vertex* v = (Vertex*)malloc(sizeof(Vertex) * (divNum + 1));
	size_t indexSize = divNum * (2 + (size_t)isFill);
	unsigned short* index = (unsigned short*)malloc(sizeof(unsigned short) * indexSize);

	Circle({ 0, 0, -h }, r, divNum, isFill, v, index);

	for (size_t i = 0; i < divNum + 1; i++)
	{
		vertices.back().vertices.emplace_back(v[i]);
	}
	for (size_t i = 0; i < indexSize; i++)
	{
		vertices.back().indices.emplace_back(index[i]);
	}

	size_t size1 = vertices.back().vertices.size();

	Circle({ 0, 0, 0.0f }, r, divNum, isFill, v, index);

	for (size_t i = 0; i < divNum + 1; i++)
	{
		vertices.back().vertices.emplace_back(v[i]);
	}
	for (size_t i = 0; i < indexSize; i++)
	{
		vertices.back().indices.emplace_back(index[i] + (unsigned short)size1);
	}

	free(v);
	free(index);

	size_t size2 = vertices.back().vertices.size();

	for (size_t i = 0; i < divNum; i++)
	{
		vertices.back().vertices.emplace_back(vertices.back().vertices[i + 0]);
		vertices.back().vertices.emplace_back(vertices.back().vertices[i + 1]);
		vertices.back().vertices.emplace_back(vertices.back().vertices[i + 0 + size1]);
		vertices.back().vertices.emplace_back(vertices.back().vertices[i + 1 + size1]);

		if (isFill == true)
		{
			vertices.back().indices.emplace_back((unsigned short)(i * 4 + 1 + size2));
			vertices.back().indices.emplace_back((unsigned short)(i * 4 + 0 + size2));
			vertices.back().indices.emplace_back((unsigned short)(i * 4 + 3 + size2));

			vertices.back().indices.emplace_back((unsigned short)(i * 4 + 3 + size2));
			vertices.back().indices.emplace_back((unsigned short)(i * 4 + 0 + size2));
			vertices.back().indices.emplace_back((unsigned short)(i * 4 + 2 + size2));
		}
		else
		{
			vertices.back().indices.emplace_back((unsigned short)(i * 4 + 0 + size2));
			vertices.back().indices.emplace_back((unsigned short)(i * 4 + 1 + size2));
			vertices.back().indices.emplace_back((unsigned short)(i * 4 + 2 + size2));
			vertices.back().indices.emplace_back((unsigned short)(i * 4 + 3 + size2));
		}
	}

	vertices.back().vertices[divNum * 4 - 3 + size2] = vertices.back().vertices[0];
	vertices.back().vertices[divNum * 4 - 1 + size2] = vertices.back().vertices[0 + size1];

	for (size_t i = 0; i < size1 - 1; i++)
	{
		vertices.back().vertices[i].pos.x *= -1;
	}

	return CreateVertexAndIndexBuffer();
}

int DrawPolygon::CreateSphere(float r, const size_t& divNum, bool isFill)
{
	using namespace DirectX;

	vertices.emplace_back(defaultVertexData);

	// 変数省略
	auto& vert = vertices.back();

	size_t sizeV = vert.vertices.size();
	size_t oldsizeV = sizeV;
	size_t sizeI = vert.indices.size();
	size_t oldsizeI = sizeI;
	size_t indexSize = divNum * 3;
	size_t divNum2 = divNum - 1;
	Vertex* v = (Vertex*)malloc(sizeof(Vertex) * (divNum + 1));
	unsigned short* index = (unsigned short*)malloc(sizeof(unsigned short) * indexSize);

	for (size_t i = 1; i < divNum2; i++)
	{
		float r2 = r * sinf(XM_PI / divNum2 * i);
		float h = r * cosf(XM_PI / divNum2 * i);

		if (r2 >= r * -0.00000005f && r2 <= r * 0.00000005f)
		{
			r2 = 0;
		}
		if (h >= r * -0.00000005f && h <= r * 0.00000005f)
		{
			h = 0;
		}

		Circle({ 0, 0, h }, r2, divNum, true, v, index);

		oldsizeV = sizeV;
		oldsizeI = sizeI;
		sizeV = vert.vertices.size();
		sizeI = vert.indices.size();

		if (i == 1)
		{
			v[divNum].pos.z = r;

			if (isFill == true)
			{
				for (size_t i = 0; i < divNum; i++)
				{
					vert.vertices.emplace_back(v[i + 0]);
					vert.vertices.emplace_back(v[i + 1]);
					vert.vertices.emplace_back(v[divNum]);

					vert.indices.emplace_back((unsigned short)(i * 3 + 1 + sizeV));
					vert.indices.emplace_back((unsigned short)(i * 3 + 0 + sizeV));
					vert.indices.emplace_back((unsigned short)(i * 3 + 2 + sizeV));
				}
				vert.vertices[(divNum * 3 - 2) + sizeV] = v[0];
			}
			else
			{
				for (size_t j = 0; j < divNum + 1; j++)
				{
					vert.vertices.emplace_back(v[j]);
				}

				for (size_t j = 0; j < divNum; j++)
				{
					vert.indices.emplace_back((unsigned short)((j + 0) + sizeV));
					vert.indices.emplace_back((unsigned short)((j + 1) + sizeV));
					vert.indices.emplace_back((unsigned short)((j + 1) + sizeV));
					vert.indices.emplace_back((unsigned short)(divNum + sizeV));
					vert.indices.emplace_back((unsigned short)(divNum + sizeV));
					vert.indices.emplace_back((unsigned short)((j + 0) + sizeV));
				}
				vert.indices[(divNum * 6 - 4) + sizeV] = (unsigned short)sizeV;
				vert.indices[(divNum * 6 - 5) + sizeV] = (unsigned short)sizeV;
			}
		}
		else
		{
			if (isFill == true)
			{
				for (size_t j = 0; j < divNum; j++)
				{
					if (i == 2)
					{
						vert.vertices.emplace_back(v[j + 0]);
						vert.vertices.emplace_back(v[j + 1]);
						vert.vertices.emplace_back(vert.vertices[j * 3 + 0 + oldsizeV]);
						vert.vertices.emplace_back(vert.vertices[j * 3 + 1 + oldsizeV]);
					}
					else
					{
						vert.vertices.emplace_back(v[j + 0]);
						vert.vertices.emplace_back(v[j + 1]);
						vert.vertices.emplace_back(vert.vertices[j * 4 + 0 + oldsizeV]);
						vert.vertices.emplace_back(vert.vertices[j * 4 + 1 + oldsizeV]);
					}

					vert.indices.emplace_back((unsigned short)((j * 4 + 1) + sizeV));
					vert.indices.emplace_back((unsigned short)((j * 4 + 0) + sizeV));
					vert.indices.emplace_back((unsigned short)((j * 4 + 3) + sizeV));

					vert.indices.emplace_back((unsigned short)((j * 4 + 3) + sizeV));
					vert.indices.emplace_back((unsigned short)((j * 4 + 0) + sizeV));
					vert.indices.emplace_back((unsigned short)((j * 4 + 2) + sizeV));
				}
				vert.vertices[divNum * 4 + sizeV - 1] = vert.vertices[oldsizeV];
				vert.vertices[divNum * 4 + sizeV - 3] = v[0];
			}
			else
			{
				for (size_t j = 0; j < divNum; j++)
				{
					vert.vertices.emplace_back(v[j]);

					vert.indices.emplace_back((unsigned short)((j + 0) + oldsizeV));
					vert.indices.emplace_back((unsigned short)((j + 0) + sizeV));
					vert.indices.emplace_back((unsigned short)((j + 0) + sizeV));
					vert.indices.emplace_back((unsigned short)((j + 1) + sizeV));
				}
				vert.indices[(divNum * 4 - 1) + sizeI] = (unsigned short)sizeV;
			}
		}
	}
	oldsizeV = sizeV;
	oldsizeI = sizeI;
	sizeV = vert.vertices.size();
	sizeI = vert.indices.size();

	v[divNum].pos.z = -r;

	if (isFill == true)
	{
		for (size_t i = 0; i < divNum; i++)
		{
			vert.vertices.emplace_back(v[i + 0]);
			vert.vertices.emplace_back(v[i + 1]);
			vert.vertices.emplace_back(v[divNum]);

			vert.indices.emplace_back((unsigned short)(i * 3 + 0 + sizeV));
			vert.indices.emplace_back((unsigned short)(i * 3 + 1 + sizeV));
			vert.indices.emplace_back((unsigned short)(i * 3 + 2 + sizeV));
		}
		vert.vertices[(divNum * 3 - 2) + sizeV] = v[0];
	}
	else
	{
		for (size_t i = 0; i < divNum; i++)
		{
			if (divNum == 3)
			{
				vert.indices.emplace_back((unsigned short)((i + 0) + oldsizeV));
				vert.indices.emplace_back((unsigned short)(divNum + 1 + oldsizeV));
			}
			else
			{
				vert.indices.emplace_back((unsigned short)((i + 0) + oldsizeV));
				vert.indices.emplace_back((unsigned short)(divNum + oldsizeV));
			}
		}
		vert.vertices.emplace_back(v[divNum]);
	}

	free(v);
	free(index);

	return CreateVertexAndIndexBuffer();
}

int DrawPolygon::CreateOBJModel(const char* filePath, const char* directoryPath)
{
	if (filePath == nullptr) { return FUNCTION_ERROR; }

	for (size_t i = 0; i < obj.size(); i++)
	{
		if (obj[i].name.compare(filePath) == 0)
		{
			return static_cast<int>(i);
		}
	}

	using namespace std;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	HRESULT hr;

	static auto* dev = DirectXInit::GetDevice();
	ifstream file;
	const string modelPath = filePath;
	file.open(modelPath);
	if (file.fail()) return FUNCTION_ERROR;

	vector<Vector3> positions;  //頂点座標
	vector<Vector3> normals;    //法線ベクトル
	vector<XMFLOAT2> texcoords; //テクスチャUV

	bool isFaceStart = false;
	size_t verticesCount = 0;
	int objectsCount = static_cast<int>(obj.size());

	Object* parent = nullptr;

	// 一行ずつ読み込む
	string line;

	while (getline(file, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得する
		std::string key;
		std::getline(line_stream, key, ' ');

		// 先頭文字列がvなら頂点座標
		if (key == "v")
		{
			// X,Y,Z座標読み込み
			Vector3 pos{};
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;
			// 座標データの追加
			positions.emplace_back(pos);
		}

		// 先頭文字列がvtならテクスチャ
		if (key == "vt")
		{
			// U,V成分読み込み
			XMFLOAT2 tex{};
			line_stream >> tex.x;
			line_stream >> tex.y;
			// V方向反転
			tex.y = 1.0f - tex.y;
			// テクスチャデータの追加
			texcoords.emplace_back(tex);
		}

		// 先頭文字列がvnなら法線ベクトル
		if (key == "vn")
		{
			// X,Y,Z成分読み込み
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			// 法線ベクトルデータの追加
			normals.emplace_back(normal);
		}

		// 先頭文字列がfならポリゴン（三角形）
		if (key == "f")
		{
			size_t index_count = 0;
			// 半角スペース区切りで行の続きを読み込む
			std::string index_string;
			while (std::getline(line_stream, index_string, ' '))
			{
				if (isFaceStart == false)
				{
					isFaceStart = true;
					vertices.emplace_back(defaultVertexData);
				}

				// 頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPos, indexUv, indexNormal;
				// インデックスの情報が頂点座標のみかどうか
				auto resultIter = std::find(index_string.begin(), index_string.end(), '/');
				auto temp = std::distance(index_string.begin(), ++resultIter);
				// 変数省略
				auto& vert = vertices.back();

				if (index_string[temp] == '/')
				{
					index_stream >> indexPos;

					// 頂点データの追加
					Vertex vertex{};
					vertex.pos = positions[indexPos - 1Ui64];
					vertex.normal = Vector3();
					vertex.uv = XMFLOAT2();
					vert.vertices.emplace_back(vertex);
				}
				else
				{
					index_stream >> indexPos;
					index_stream.seekg(1, std::ios_base::cur); //スラッシュを飛ばす
					index_stream >> indexUv;
					index_stream.seekg(1, std::ios_base::cur); //スラッシュを飛ばす
					index_stream >> indexNormal;

					// 頂点データの追加
					Vertex vertex{};
					vertex.pos = positions[indexPos - 1Ui64];
					vertex.normal = normals[indexNormal - 1Ui64];
					vertex.uv = texcoords[indexUv - 1Ui64];
					vert.vertices.emplace_back(vertex);
				}

				// インデックスデータの追加
				vert.indices.emplace_back((unsigned short)vert.vertices.size() - 1);
				index_count++;

				if (index_count > 3)
				{
					vert.indices.emplace_back((unsigned short)(vert.vertices.size() - index_count));
					vert.indices.emplace_back(vert.indices[vert.indices.size() - 2] - 1);
				}
			}
		}

		// 先頭文字列がmtllibならマテリアルファイルの読み込み
		if (key == "mtllib")
		{
			// マテリアルのファイル名読み込み
			std::string fileName;
			line_stream >> fileName;
			// マテリアル読み込み
			LoadMaterial(directoryPath, fileName);
		}

		// 先頭文字列がusemtlなら使用するマテリアル
		if (key == "usemtl")
		{
			// マテリアル名読み込み
			std::string materialName;
			line_stream >> materialName;

			size_t materialIndex = 0;
			for (materialIndex = 0; materialIndex < material.size(); materialIndex++)
			{
				if (materialName == material[materialIndex].name)
				{
					break;
				}
			}
			if (materialIndex >= material.size())
			{
				return FUNCTION_ERROR;
			}

			isFaceStart = false;
			obj.emplace_back(OBJData{});
			obj.back().name = filePath;
			if (parent == nullptr)
			{
				parent = &obj.back();
			}
			else
			{
				obj.back().parent = parent;
			}
			obj.back().polygonData = (int)(vertices.size());
			obj.back().material = material[materialIndex];
			objModelCount++;
			verticesCount++;
		}
	}
	// ファイルを閉じる
	file.close();

	if (obj.back().polygonData == FUNCTION_ERROR)
	{
		return FUNCTION_ERROR;
	}

	// 頂点データ一つ分のサイズ * 頂点データの要素数
	for (size_t i = vertices.size() - verticesCount; i < vertices.size(); i++)
	{
#pragma region VertexBuffer

		UINT sizeVB;
		sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices[i].vertices.size());

		hr = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertices[i].vertBuff));
		if (FAILED(hr))
		{
			return FUNCTION_ERROR;
		}

		hr = vertices[i].vertBuff->Map(0, nullptr, (void**)&vertMap);

		// 全頂点に対して
		for (UINT j = 0; j < vertices[i].vertices.size(); j++)
		{
			vertMap[j] = vertices[i].vertices[j]; //座標をコピー
		}

		// マップを解除
		vertices[i].vertBuff->Unmap(0, nullptr);

		vertices[i].vbView.BufferLocation = vertices[i].vertBuff->GetGPUVirtualAddress();
		vertices[i].vbView.SizeInBytes = sizeVB;
		vertices[i].vbView.StrideInBytes = sizeof(Vertex);

#pragma endregion //VertexBuffer

#pragma region IndexBuffer

		//インデックスデータ全体のサイズ
		UINT sizeIB;
		sizeIB = static_cast<UINT>(sizeof(unsigned short) * vertices[i].indices.size());

		hr = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertices[i].indexBuff));
		if (FAILED(hr))
		{
			return FUNCTION_ERROR;
		}

		// GPU上のバッファに対応した仮想メモリを取得
		static unsigned short* indexMap = nullptr;
		hr = vertices[i].indexBuff->Map(0, nullptr, (void**)&indexMap);

		// 全インデックスに対して
		for (size_t j = 0; j < vertices[i].indices.size(); j++)
		{
			indexMap[j] = vertices[i].indices[j]; //インデックスをコピー
		}

		// 繋がりを解除
		vertices[i].indexBuff->Unmap(0, nullptr);

		vertices[i].ibView.BufferLocation = vertices[i].indexBuff->GetGPUVirtualAddress();
		vertices[i].ibView.Format = DXGI_FORMAT_R16_UINT;
		vertices[i].ibView.SizeInBytes = sizeIB;

#pragma endregion //IndexBuffer
	}

	for (size_t i = obj.size() - verticesCount; i < obj.size(); i++)
	{
		hr = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xFF) & ~0xFF), //リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&obj[i].constBuff));
		if (FAILED(hr))
		{
			return hr;
		}
		hr = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(MaterialConstBufferData) + 0xFF) & ~0xFF), //リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&obj[i].materialConstBuff));
		if (FAILED(hr))
		{
			return hr;
		}
	}

	return (int)(obj.size() - 1);
}

void DrawPolygon::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	// ファイルストリーム
	std::ifstream file;
	// マテリアルファイルを開く
	file.open(directoryPath + filename);
	// ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	ContainerClear(material);

	// 1行ずつ読み込む
	std::string line;
	while (std::getline(file, line))
	{
		// 1行分の文字列をストリームに変換
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		std::getline(line_stream, key, ' ');

		// 先頭のタブは無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin()); //先頭の文字を削除
		}

		// 先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{
			material.emplace_back(Material{});
			// マテリアル名読み込み
			line_stream >> material[material.size() - 1].name;
		}

		// 先頭文字列がKaならアンビエント色
		if (key == "Ka")
		{
			line_stream >> material[material.size() - 1].ambient.x;
			line_stream >> material[material.size() - 1].ambient.y;
			line_stream >> material[material.size() - 1].ambient.z;
		}

		// 先頭文字列がKdならディフューズ色
		if (key == "Kd")
		{
			line_stream >> material[material.size() - 1].diffuse.x;
			line_stream >> material[material.size() - 1].diffuse.y;
			line_stream >> material[material.size() - 1].diffuse.z;
		}

		// 先頭文字列がKsならスペキュラー色
		if (key == "Ks")
		{
			line_stream >> material[material.size() - 1].specular.x;
			line_stream >> material[material.size() - 1].specular.y;
			line_stream >> material[material.size() - 1].specular.z;
		}

		// 先頭文字列がdかTrならアルファ
		if (key == "d" || key == "Tr")
		{
			line_stream >> material[material.size() - 1].alpha;
		}

		// 先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd")
		{
			// テクスチャのファイル名読み込み
			line_stream >> material[material.size() - 1].textureFilename;
			std::string texFilePath = directoryPath + material[material.size() - 1].textureFilename;
			// ユニコード文字列に変換する
			wchar_t filepath[128];
			int iBufferSize = MultiByteToWideChar(CP_ACP, 0, texFilePath.c_str(), -1, filepath, _countof(filepath));
			// テクスチャ読み込み
			material[material.size() - 1].textrueIndex = LoadTextrue(filepath);
		}
	}
	// ファイルを閉じる
	file.close();
}

int DrawPolygon::DeleteModelData(int* modelData)
{
	if (*modelData < 0 || *modelData >= static_cast<int>(vertices.size()))
	{
		return FUNCTION_ERROR;
	}

	for (size_t i = 0; i < obj.size(); i++)
	{
		if (*modelData == obj[i].polygonData)
		{
		}
	}

	return 0;
}

void DrawPolygon::Tiring(int polygonData, float uvScaleX, float uvScaleY)
{
	if (polygonData < 0 || (size_t)polygonData >= vertices.size())
	{
		return;
	}

	for (size_t i = 0; i < vertices[polygonData].vertices.size(); i++)
	{
		vertices[polygonData].vertices[i].uv.x *= uvScaleX;
		vertices[polygonData].vertices[i].uv.y *= uvScaleY;
	}

	vertices[polygonData].vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (UINT i = 0; i < vertices[polygonData].vertices.size(); i++)
	{
		vertMap[i] = vertices[polygonData].vertices[i]; //座標をコピー
	}

	// マップを解除
	vertices[polygonData].vertBuff->Unmap(0, nullptr);
}

void DrawPolygon::AutoTiring(int polygonData, int graphHandle)
{
	if ((polygonData < 0 || (size_t)polygonData >= vertices.size()) ||
		(graphHandle < 0 || (size_t)graphHandle >= textrueData.size()))
	{
		return;
	}

	// テクスチャデータ取得
	D3D12_RESOURCE_DESC resDesc = textrueData[graphHandle].texbuff->GetDesc();
	float left = 0.0f, right = 0.0f, top = 0.0f, bottom = 0.0f;

	for (size_t i = 0; i < vertices[polygonData].vertices.size(); i++)
	{
		if (vertices[polygonData].vertices[i].pos.x < left)
		{
			left = vertices[polygonData].vertices[i].pos.x;
		}
		if (vertices[polygonData].vertices[i].pos.x > right)
		{
			right = vertices[polygonData].vertices[i].pos.x;
		}
		if (vertices[polygonData].vertices[i].pos.y < top)
		{
			top = vertices[polygonData].vertices[i].pos.y;
		}
		if (vertices[polygonData].vertices[i].pos.y > bottom)
		{
			bottom = vertices[polygonData].vertices[i].pos.y;
		}
	}
	DirectX::XMFLOAT2 size = { right - left, bottom - top };
	size.x /= resDesc.Width;
	size.y /= resDesc.Height;

	for (size_t i = 0; i < vertices[polygonData].vertices.size(); i++)
	{
		vertices[polygonData].vertices[i].uv.x *= size.x;
		vertices[polygonData].vertices[i].uv.y *= size.y;
	}

	vertices[polygonData].vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (UINT i = 0; i < vertices[polygonData].vertices.size(); i++)
	{
		vertMap[i] = vertices[polygonData].vertices[i]; //座標をコピー
	}

	// マップを解除
	vertices[polygonData].vertBuff->Unmap(0, nullptr);
}

void DrawPolygon::MoveUV(int polygonData, int graphHandle, DirectX::XMFLOAT2 speed)
{
	if ((polygonData < 0 || (size_t)polygonData >= vertices.size()) ||
		(graphHandle < 0 || (size_t)graphHandle >= textrueData.size()))
	{
		return;
	}

	// テクスチャデータ取得
	D3D12_RESOURCE_DESC resDesc = textrueData[graphHandle].texbuff->GetDesc();
	DirectX::XMFLOAT2 addUV = { speed.x / resDesc.Width, speed.y / resDesc.Height };

	for (size_t i = 0; i < vertices[polygonData].vertices.size(); i++)
	{
		vertices[polygonData].vertices[i].uv.x += addUV.x;
		vertices[polygonData].vertices[i].uv.y += addUV.y;
	}

	vertices[polygonData].vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (UINT i = 0; i < vertices[polygonData].vertices.size(); i++)
	{
		vertMap[i] = vertices[polygonData].vertices[i]; //座標をコピー
	}

	// マップを解除
	vertices[polygonData].vertBuff->Unmap(0, nullptr);
}

int DrawPolygon::DrawPolygonInit()
{
	HRESULT hr = S_FALSE;
	static bool isDrawPolygonInit = false;

	int index = (int)obj.size();
	hr = CreateConstBuffer(&index);
	if (FAILED(hr))
	{
		return FUNCTION_ERROR;
	}

	if (isDrawPolygonInit == false)
	{
		isDrawPolygonInit = true;

		if (FAILED(CreateDescriptorHeap()))
		{
			return FUNCTION_ERROR;
		}
	}

	return index;
}

int DrawPolygon::DrawOBJInit()
{
	HRESULT hr = S_FALSE;

	int index = (int)obj.size();
	hr = CreateConstBuffer(&index);
	if (FAILED(hr))
	{
		return FUNCTION_ERROR;
	}

	if (isDrawOBJPolygonInit == false)
	{
		isDrawOBJPolygonInit = true;
	}

	return index;
}

int DrawPolygon::Draw(
	int polygonData, const Vector3& position, const Matrix4& rotation, const Vector3& scale,
	const XMFLOAT4& color, int graphHandle, bool isAlpha,
	bool isOrthoGraphic, bool viewMultiFlag, Object* parent)
{
	if ((polygonData < 0 || (size_t)polygonData >= vertices.size()) ||
		(graphHandle < 0 || (UINT)graphHandle > textrueCount))
	{
		return FUNCTION_ERROR;
	}

	using namespace DirectX;

	static auto* cmdList = DirectXInit::GetCommandList();
	bool isInit = false;
	XMMATRIX mat = XMMatrixIdentity();

	if ((size_t)(polygonCount + 1) < objIndex.size())
	{
		isInit = true;
	}

	if (isInit == false)
	{
		int size = DrawPolygonInit();
		if (size == FUNCTION_ERROR)
		{
			return FUNCTION_ERROR;
		}

		objIndex.emplace_back(IndexData{ size, graphHandle });
	}

	if (objIndex.size() <= 0)
	{
		return FUNCTION_ERROR;
	}

	polygonCount++;
	polygonCount %= objIndex.size();

	IndexData& index = objIndex[polygonCount];

	UpdataConstant(position, rotation, scale, color, index.constant, polygonData, parent);

	if (index.textrue != graphHandle)
	{
		index.textrue = graphHandle;
	}

	if (viewMultiFlag == true)
	{
		mat = Camera::GetMatView();
	}
	else
	{
		mat = XMMatrixIdentity();
	}

	if (isOrthoGraphic == true)
	{
		mat *= Camera::matProjection[Camera::Projection::ORTHOGRAPHIC];
		lightVec = {
			-vertices[polygonData].vertices[0].normal.x,
			-vertices[polygonData].vertices[0].normal.y,
			-vertices[polygonData].vertices[0].normal.z
		};
	}
	else
	{
		mat *= Camera::matProjection[Camera::Projection::PERSPECTIVE];
		lightVec = light;
	}

	if (isAlpha)
	{
		alphaObj.push_back(
			{
				polygonCount,
				polygonData,
				obj[index.constant].matWorld * mat,
				lightVec,
				false
			});

		return index.constant;
	}

#pragma region GraphicsCommand

	BaseDrawGraphics();

	ConstBufferData* constMap = nullptr;
	obj[index.constant].constBuff->Map(0, nullptr, (void**)&constMap); //マッピング

	constMap->color = obj[index.constant].color;
	constMap->mat = obj[index.constant].matWorld * mat;
	constMap->lightVec = lightVec;
	obj[index.constant].constBuff->Unmap(0, nullptr); //マッピング解除

	// デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeaps[] = { texCommonData.descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, obj[index.constant].constBuff->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootDescriptorTable(1, textrueData[index.textrue].gpuDescHandle);

	// プリミティブ形状の設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vertices[polygonData].vbView);
	// インデックスバッファビューの設定
	cmdList->IASetIndexBuffer(&vertices[polygonData].ibView);
	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)vertices[polygonData].indices.size(), 1, 0, 0, 0);

#pragma endregion //GraphicsCommand

	return index.constant;
}

int DrawPolygon::DrawOBJ(int object, const Vector3& position, const Matrix4& rotation, const Vector3& scale,
						 const XMFLOAT4& color, bool isAlpha, bool isOrthoGraphic, bool viewMultiFlag, Object* parent)
{
	if ((object < 0 || object >= obj.size()) ||
		(obj[object].polygonData < 0 || (size_t)obj[object].polygonData >= vertices.size()) ||
		(obj[object].material.textrueIndex < 0 || (UINT)obj[object].material.textrueIndex > textrueCount))
	{
		return FUNCTION_ERROR;
	}

	using namespace DirectX;

	bool isInit = false;
	XMMATRIX mat = XMMatrixIdentity();
	int parentIndex = object;

	for (int i = object; i >= 0; i--)
	{
		if ((size_t)(polygonCount + 1) < objIndex.size())
		{
			isInit = true;
		}

		if (isInit == false)
		{
			int size = DrawOBJInit();
			if (size == FUNCTION_ERROR)
			{
				return FUNCTION_ERROR;
			}

			objIndex.emplace_back(IndexData{ size, (int)obj[i].material.textrueIndex });
		}

		if (obj[i].parent != nullptr)
		{
			parentIndex = i;
			break;
		}
	}

	if (objIndex.size() <= 0)
	{
		return FUNCTION_ERROR;
	}

	polygonCount++;
	polygonCount %= objIndex.size();

	IndexData& index = objIndex[polygonCount];

	for (int i = parentIndex; i <= object; i++)
	{
		if (i == parentIndex)
		{
			UpdataConstant(position, rotation, scale, color, index.constant, obj[i].polygonData, parent);
		}
		else
		{
			UpdataConstant(position, rotation, scale, color, index.constant, obj[i].polygonData, obj[i].parent);
		}
	}

	if (viewMultiFlag == true)
	{
		mat = Camera::GetMatView();
	}
	else
	{
		mat = XMMatrixIdentity();
	}

	if (isOrthoGraphic == true)
	{
		mat *= Camera::matProjection[Camera::Projection::ORTHOGRAPHIC];
		lightVec = {
			-vertices[obj[object].polygonData].vertices[0].normal.x,
			-vertices[obj[object].polygonData].vertices[0].normal.y,
			-vertices[obj[object].polygonData].vertices[0].normal.z
		};
	}
	else
	{
		mat *= Camera::matProjection[Camera::Projection::PERSPECTIVE];
		lightVec = {
			-vertices[obj[object].polygonData].vertices[0].normal.x,
			-vertices[obj[object].polygonData].vertices[0].normal.y,
			-vertices[obj[object].polygonData].vertices[0].normal.z
		};
	}

	if (isAlpha)
	{
		alphaObj.push_back(
			{
				polygonCount,
				object,
				obj[index.constant].matWorld * mat,
				lightVec,
				true,
				parentIndex
			});

		return parentIndex;
	}

#pragma region GraphicsCommand

	BaseDrawGraphics();

	static auto* cmdList = DirectXInit::GetCommandList();

	for (int i = parentIndex; i <= object; i++)
	{
		ConstBufferData* constMap = nullptr;
		obj[index.constant].constBuff->Map(0, nullptr, (void**)&constMap); //マッピング

		constMap->color = obj[index.constant].color;
		constMap->mat = obj[index.constant].matWorld * mat;
		constMap->lightVec = lightVec;
		obj[index.constant].constBuff->Unmap(0, nullptr); //マッピング解除

		MaterialConstBufferData* materialConstMap = nullptr;
		obj[index.constant].materialConstBuff->Map(0, nullptr, (void**)&materialConstMap); //マッピング

		materialConstMap->ambient = obj[i].material.ambient;
		materialConstMap->diffuse = obj[i].material.diffuse;
		materialConstMap->specular = obj[i].material.specular;
		materialConstMap->alpha = obj[i].material.alpha;
		obj[index.constant].materialConstBuff->Unmap(0, nullptr); //マッピング解除

		// デスクリプタヒープをセット
		ID3D12DescriptorHeap* ppHeaps[] = { texCommonData.descHeap.Get() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		// 定数バッファビューをセット
		cmdList->SetGraphicsRootConstantBufferView(0, obj[index.constant].constBuff->GetGPUVirtualAddress());
		cmdList->SetGraphicsRootConstantBufferView(1, obj[index.constant].materialConstBuff->GetGPUVirtualAddress());
		cmdList->SetGraphicsRootDescriptorTable(2, textrueData[obj[i].material.textrueIndex].gpuDescHandle);

		// プリミティブ形状の設定
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 頂点バッファの設定
		cmdList->IASetVertexBuffers(0, 1, &vertices[obj[i].polygonData].vbView);
		// インデックスバッファビューの設定
		cmdList->IASetIndexBuffer(&vertices[obj[i].polygonData].ibView);
		// 描画コマンド
		cmdList->DrawIndexedInstanced((UINT)vertices[obj[i].polygonData].indices.size(), 1, 0, 0, 0);
	}

#pragma endregion //GraphicsCommand

	return parentIndex;
}

void DrawPolygon::DrawAlpha()
{
	using namespace DirectX;

#pragma region GraphicsCommand

	static auto* cmdList = DirectXInit::GetCommandList();
	BaseDrawGraphics();

	for (auto& i : alphaObj)
	{
		IndexData& index = objIndex[i.polygonIndex];

		if (i.isObj)
		{
			for (int j = i.parentIndex; j <= i.objectIndex; j++)
			{
				ChangeMaterialShader();

				ConstBufferData* constMap = nullptr;
				obj[index.constant].constBuff->Map(0, nullptr, (void**)&constMap); //マッピング

				constMap->color = obj[index.constant].color;
				constMap->mat = i.mat;
				constMap->lightVec = i.lightVec;
				obj[index.constant].constBuff->Unmap(0, nullptr); //マッピング解除

				MaterialConstBufferData* materialConstMap = nullptr;
				obj[index.constant].materialConstBuff->Map(0, nullptr, (void**)&materialConstMap); //マッピング

				materialConstMap->ambient = obj[j].material.ambient;
				materialConstMap->diffuse = obj[j].material.diffuse;
				materialConstMap->specular = obj[j].material.specular;
				materialConstMap->alpha = obj[j].material.alpha;
				obj[index.constant].materialConstBuff->Unmap(0, nullptr); //マッピング解除

				// デスクリプタヒープをセット
				ID3D12DescriptorHeap* ppHeaps[] = { texCommonData.descHeap.Get() };
				cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

				// 定数バッファビューをセット
				cmdList->SetGraphicsRootConstantBufferView(0, obj[index.constant].constBuff->GetGPUVirtualAddress());
				cmdList->SetGraphicsRootConstantBufferView(1, obj[index.constant].materialConstBuff->GetGPUVirtualAddress());
				cmdList->SetGraphicsRootDescriptorTable(2, textrueData[obj[j].material.textrueIndex].gpuDescHandle);

				// プリミティブ形状の設定
				cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				// 頂点バッファの設定
				cmdList->IASetVertexBuffers(0, 1, &vertices[obj[j].polygonData].vbView);
				// インデックスバッファビューの設定
				cmdList->IASetIndexBuffer(&vertices[obj[j].polygonData].ibView);
				// 描画コマンド
				cmdList->DrawIndexedInstanced((UINT)vertices[obj[j].polygonData].indices.size(), 1, 0, 0, 0);
			}
		}
		else
		{
			ChangeOBJShader();

			ConstBufferData* constMap = nullptr;
			obj[index.constant].constBuff->Map(0, nullptr, (void**)&constMap); //マッピング

			constMap->color = obj[index.constant].color;
			constMap->mat = i.mat;
			constMap->lightVec = i.lightVec;
			obj[index.constant].constBuff->Unmap(0, nullptr); //マッピング解除

			// デスクリプタヒープをセット
			ID3D12DescriptorHeap* ppHeaps[] = { texCommonData.descHeap.Get() };
			cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

			// 定数バッファビューをセット
			cmdList->SetGraphicsRootConstantBufferView(0, obj[index.constant].constBuff->GetGPUVirtualAddress());
			cmdList->SetGraphicsRootDescriptorTable(1, textrueData[index.textrue].gpuDescHandle);

			// プリミティブ形状の設定
			cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			// 頂点バッファの設定
			cmdList->IASetVertexBuffers(0, 1, &vertices[i.objectIndex].vbView);
			// インデックスバッファビューの設定
			cmdList->IASetIndexBuffer(&vertices[i.objectIndex].ibView);
			// 描画コマンド
			cmdList->DrawIndexedInstanced((UINT)vertices[i.objectIndex].indices.size(), 1, 0, 0, 0);
		}
	}

	alphaObj.clear();

#pragma endregion //GraphicsCommand
}

int DrawPolygon::CreateCamera(const Vector3& cameraPos, const Vector3& cameraTarget, const Vector3& upVector)
{
	using namespace DirectX;

	Camera::matView.emplace_back(Camera::CreateCamera(
		XMLoadFloat3(&cameraPos),
		XMLoadFloat3(&cameraTarget),
		XMLoadFloat3(&upVector)
	));

	return (int)Camera::matView.size() - 1;
}

DirectX::XMFLOAT3 DrawPolygon::ScreenToWorld(int x, int y, float z)
{
	using namespace DirectX;

	XMFLOAT3 world = {};
	XMFLOAT3 screen = { (float)x, (float)y, z };
	XMMATRIX InvView, InvPrj, VP, InvViewport;

	InvView = XMMatrixInverse(nullptr, Camera::matView[Camera::cameraNo]);
	InvPrj = XMMatrixInverse(nullptr, Camera::matProjection[Camera::Projection::PERSPECTIVE]);
	VP = XMMatrixIdentity();
	VP.r[0].m128_f32[0] =
		static_cast<float>(DirectXInit::GetInstance()->windowWidth) / 2.0f;
	VP.r[1].m128_f32[1] =
		-(static_cast<float>(DirectXInit::GetInstance()->windowHeight)) / 2.0f;
	VP.r[3].m128_f32[0] =
		static_cast<float>(DirectXInit::GetInstance()->windowWidth) / 2.0f;
	VP.r[3].m128_f32[1] =
		static_cast<float>(DirectXInit::GetInstance()->windowHeight) / 2.0f;
	InvViewport = XMMatrixInverse(nullptr, VP);

	XMMATRIX temp = InvViewport * InvPrj * InvView;
	XMStoreFloat3(&world, XMVector3TransformCoord(XMLoadFloat3(&screen), temp));

	return world;
}

DirectX::XMMATRIX DrawPolygon::InverseView()
{
	DirectX::XMMATRIX invView = DirectX::XMMatrixInverse(nullptr, Camera::matView[Camera::cameraNo]);
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;
	return invView;
}

void DrawPolygon::PolygonLoopEnd()
{
	loopCount++;

	TextLoopEnd();

	polygonCount = static_cast<int>(objModelCount) - 1;
}

void DrawPolygon::DataClear()
{
	ContainerClear(verticesCount);
	ContainerClear(material);

#ifdef _DEBUG
	OutputDebugStringA("'DrawPolygon'のデータを削除しました。\n");
#endif // _DEBUG
}

void DrawPolygon::Circle(const Vector3& centerPos, float r, const size_t& divNum,
						 bool flag, Vertex* v, unsigned short* index)
{
	using namespace DirectX;

	if (v == nullptr || index == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < divNum; i++)
	{
		/*頂点座標*/
		v[i].pos.x = r * sinf(2 * XM_PI / divNum * i) + centerPos.x;
		v[i].pos.y = r * cosf(2 * XM_PI / divNum * i) + centerPos.y;
		v[i].pos.z = centerPos.z;
		v[i].normal = {};
		v[i].uv.x = (sinf(2 * XM_PI / divNum * i) + 1.0f) / 2.0f;
		v[i].uv.y = (cosf(2 * XM_PI / divNum * i) + 1.0f) / 2.0f;

		if (v[i].pos.x >= r * -0.00000005f && v[i].pos.x <= r * 0.00000005f)
		{
			v[i].pos.x = 0;
		}
		if (v[i].pos.y >= r * -0.00000005f && v[i].pos.y <= r * 0.00000005f)
		{
			v[i].pos.y = 0;
		}
		if (v[i].uv.x >= -0.00000005f && v[i].uv.x <= 0.00000005f)
		{
			v[i].uv.x = 0;
		}
		if (v[i].uv.y >= -0.00000005f && v[i].uv.y <= 0.00000005f)
		{
			v[i].uv.y = 0;
		}

		if (flag == true)
		{
			/*fill用インデックスデータ*/
			index[i * 3 + 0] = (unsigned short)(i + 1);
			index[i * 3 + 1] = (unsigned short)(i + 0);
			index[i * 3 + 2] = (unsigned short)(divNum);
		}
		else
		{
			/*line用インデックスデータ*/
			index[i * 2 + 0] = (unsigned short)(i + 1);
			index[i * 2 + 1] = (unsigned short)(i + 0);
		}
	}

	// verticesのdivNum番目にcenterPosを代入
	v[divNum].pos = { centerPos.x, centerPos.y, centerPos.z };
	v[divNum].normal = {};
	v[divNum].uv = { 0.5f, 0.5f };

	for (size_t i = 0; i < divNum + 1; i++)
	{
		v[i].uv.y *= -1;
	}

	/*インデックスデータ調整*/
	if (flag == true)
	{
		/*fill用インデックスデータ*/
		index[(divNum - 1) * 3] = (unsigned short)0; //一番最後の手前の要素を'0'に置き換え
	}
	else
	{
		/*line用インデックスデータ*/
		index[(divNum - 1) * 2] = (unsigned short)0; //一番最後の要素を'0'に置き換え
	}

	return;
}
} //Library
} //Engine
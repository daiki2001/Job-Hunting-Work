#include "BlockType.h"

const int BlockType::WIDTH = 32;
const int BlockType::HEIGHT = 32;
const std::string BlockType::blockResourcesDir = "./Resources/Game/Block/";
DrawPolygon* BlockType::draw = nullptr;

BlockType::BlockType(const int& typeId, DrawPolygon* const draw) :
	typeId(typeId),
	graph(FUNCTION_ERROR),
	blockBox(FUNCTION_ERROR),
	scale(1.0f, 1.0f, 1.0f),
	color(1.0f, 1.0f, 1.0f, 1.0f)
{
	if (this->draw == nullptr)
	{
		this->draw = draw;
	}
}

BlockType::~BlockType()
{
}

int BlockType::Create(const wchar_t* filename, const Math::Matrix4& rotation,
					  const Math::Vector3& scale, const DirectX::XMFLOAT4& color)
{
	if (filename == nullptr)
	{
		graph = 0;
	}
	else
	{
		const std::wstring wBlockResourcesDir = L"./Resources/Game/Block/";
		auto filePath = wBlockResourcesDir.c_str() + static_cast<std::wstring>(filename);
		graph = draw->LoadTextrue(filePath.c_str());
	}

	if (blockBox == FUNCTION_ERROR)
	{
		blockBox = draw->Create3Dbox(1.0f, 1.0f, 1.0f);
	}

	this->rotation = rotation;
	this->scale = scale;
	this->color = color;

	return graph;
}

int BlockType::Create(const char* filename, const Math::Matrix4& rotation,
					  const Math::Vector3& scale, const DirectX::XMFLOAT4& color)
{
	graph = FUNCTION_ERROR;

	if (blockBox == FUNCTION_ERROR)
	{
		auto resDir = blockResourcesDir + ExtractDirectory(filename);
		blockBox = draw->CreateOBJModel((blockResourcesDir + filename).c_str(), resDir.c_str());
	}

	this->rotation = rotation;
	this->scale = scale;
	this->color = color;

	return blockBox;
}

void BlockType::Draw(const Vector3& pos)
{
	// 'typeId'が0(None)は描画しない
	if (typeId == 0)
	{
		return;
	}

	if (graph == FUNCTION_ERROR)
	{
		// 'graph'が'FUNCTION_ERROR'の時
		DirectDrawing::ChangeMaterialShader();
		draw->DrawOBJ(blockBox, pos, rotation, scale, color);
	}
	else
	{
		// 'graph'が'FUNCTION_ERROR'でない時
		DirectDrawing::ChangeOBJShader();
		draw->Draw(blockBox, pos, rotation, scale, color, graph);
	}
}

#ifdef _DEBUG
void BlockType::Draw(const Vector3& pos, const DirectX::XMFLOAT4& color)
{
	// 'typeId'が0(None)は描画しない
	if (typeId == 0)
	{
		return;
	}

	if (graph == FUNCTION_ERROR)
	{
		// 'graph'が'FUNCTION_ERROR'の時
		draw->DrawOBJ(blockBox, pos, Math::Identity(), Vector3(1.0f, 1.0f, 1.0f), color);
	}
	else
	{
		// 'graph'が'FUNCTION_ERROR'でない時
		draw->Draw(blockBox, pos, Math::Identity(), Vector3(1.0f, 1.0f, 1.0f), color, graph);
	}
}
#endif // _DEBUG


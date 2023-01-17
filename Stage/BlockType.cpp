#include "BlockType.h"

const float BlockType::BLOCK_SIZE = 1.0f;
const float BlockType::BLOCK_HEIGHT = 1.5f;
const float BlockType::FLOOR_HEIGHT = 0.5f;
const std::string BlockType::blockResourcesDir = "./Resources/Game/Block/";
DrawPolygon* BlockType::draw = nullptr;
int BlockType::floorGraph = FUNCTION_ERROR;
int BlockType::floorObj = FUNCTION_ERROR;

BlockType::BlockType(int typeId) :
	typeId(typeId),
	graph(FUNCTION_ERROR),
	blockBox(FUNCTION_ERROR),
	rotation(Math::Identity()),
	scale(1.0f, 1.0f, 1.0f),
	color(1.0f, 1.0f, 1.0f, 1.0f)
{
}

BlockType::~BlockType()
{
}

void BlockType::StaticInit(DrawPolygon* const draw)
{
	if (BlockType::draw == nullptr)
	{
		BlockType::draw = draw;
	}

	if (floorGraph == FUNCTION_ERROR)
	{
		floorGraph = BlockType::draw->LoadTextrue(L"./Resources/Game/Floor.png");
	}
	if (floorObj == FUNCTION_ERROR)
	{
		floorObj = BlockType::draw->Create3Dbox(Vector3(BLOCK_SIZE, BLOCK_SIZE, FLOOR_HEIGHT));
	}
}

int BlockType::Create(const wchar_t* filename, const Matrix4& rotation, const Vector3& scale, const XMFLOAT4& color)
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
		blockBox = draw->Create3Dbox(scale_xyz(BLOCK_SIZE));
	}

	this->rotation = rotation;
	this->scale = scale;
	this->color = color;

	return graph;
}

int BlockType::Create(const char* filename, const Matrix4& rotation, const Vector3& scale, const XMFLOAT4& color)
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

int BlockType::Create(int number, bool isObject, const Matrix4& rotation, const Vector3& scale, const XMFLOAT4& color)
{
	if (isObject)
	{
		graph = FUNCTION_ERROR;
		blockBox = number;
	}
	else
	{
		graph = number;

		if (blockBox == FUNCTION_ERROR)
		{
			blockBox = draw->Create3Dbox(scale_xyz(BLOCK_SIZE));
		}
	}

	this->rotation = rotation;
	this->scale = scale;
	this->color = color;

	return blockBox;
}

void BlockType::Draw(const Vector3& pos)
{
	// 床の描画
	Vector3 floorPos = pos;
	floorPos.z += (BLOCK_HEIGHT + FLOOR_HEIGHT) / 2.0f;
	DirectDrawing::ChangeOBJShader();
	draw->Draw(floorObj, floorPos, Math::Identity(), scale_xyz(1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), floorGraph);

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
		draw->Draw(blockBox, pos, rotation, scale, color, graph);
	}
}

#include "BlockType.h"
#include "BlockManager.h"
#include "./Header/DrawPolygon.h"
#include "LoadGraph.h"
#include "./Header/Parameter.h"

const float BlockType::BLOCK_SIZE = 1.0f;
const float BlockType::FLOOR_HEIGHT = 1.0f;
const std::string BlockType::blockResourcesDir = "./Resources/Game/Block/";
int BlockType::floorGraph = FUNCTION_ERROR;
int BlockType::whiteTile = FUNCTION_ERROR;
int BlockType::floorBox = FUNCTION_ERROR;
int BlockType::switchBlock = FUNCTION_ERROR;
int BlockType::blueSwitchBlock = FUNCTION_ERROR;

BlockType::BlockType(int typeId) :
	typeId(typeId),
	graph(FUNCTION_ERROR),
	blockBox(FUNCTION_ERROR),
	rotation(Math::Identity()),
	scale(1.0f, 1.0f, 1.0f),
	color(Color::AddAlphaValue(Color::WHITE, 1.0f))
{
}

BlockType::~BlockType()
{
}

void BlockType::StaticInit()
{
	auto draw = Library::DrawPolygon::GetInstance();

	if (floorGraph == FUNCTION_ERROR)
	{
		floorGraph = draw->LoadTextrue(L"./Resources/Game/Floor.png");
	}
	if (whiteTile == FUNCTION_ERROR)
	{
		whiteTile = Parameter::Get(LoadGraph::WHITE_TILE.c_str());
	}
	if (floorBox == FUNCTION_ERROR)
	{
		floorBox = draw->Create3Dbox(Vector3::Scale_xyz(BLOCK_SIZE));
	}
	if (switchBlock == FUNCTION_ERROR)
	{
		switchBlock = draw->CreateOBJModel("./Resources/Game/Block/Switch.obj", "./Resources/Game/Block/");
	}
	if (blueSwitchBlock == FUNCTION_ERROR)
	{
		blueSwitchBlock = draw->CreateOBJModel("./Resources/Game/Block/Switch_blue.obj", "./Resources/Game/Block/");
	}
}

int BlockType::Create(const wchar_t* filename, const Matrix4& rotation, const Vector3& scale, const XMFLOAT4& color)
{
	if (filename == nullptr)
	{
		return 0;
	}
	else
	{
		const std::wstring wBlockResourcesDir = L"./Resources/Game/Block/";
		auto filePath = wBlockResourcesDir.c_str() + static_cast<std::wstring>(filename);
		graph = Library::DrawPolygon::GetInstance()->LoadTextrue(filePath.c_str());
	}

	if (blockBox == FUNCTION_ERROR)
	{
		blockBox = floorBox;
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
		blockBox = Library::DrawPolygon::GetInstance()->CreateOBJModel((blockResourcesDir + filename).c_str(), resDir.c_str());
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
			blockBox = floorBox;
		}
	}

	this->rotation = rotation;
	this->scale = scale;
	this->color = color;

	return blockBox;
}

void BlockType::Draw(const Vector3& offset) const
{
	auto draw = Library::DrawPolygon::GetInstance();

	if (graph == FUNCTION_ERROR)
	{
		// 'graph'が'FUNCTION_ERROR'の時
		int objIndex = blockBox;
		if (blockBox == switchBlock && BlockManager::GetBlockSwitch())
		{
			objIndex = blueSwitchBlock;
		}

		DirectDrawing::ChangeMaterialShader();
		draw->DrawOBJ(objIndex, offset, rotation, scale, color);
	}
	else
	{
		// 'graph'が'FUNCTION_ERROR'でない時
		DirectDrawing::ChangeOBJShader();
		draw->Draw(blockBox, offset, rotation, scale, color, graph);
	}
}

void BlockType::FloorDraw(const Vector3& offset, bool isWhiteTile)
{
	Vector3 floorPos = offset;
	floorPos += Vector3(0.0f, 0.0f, FLOOR_HEIGHT);
	int graph = FUNCTION_ERROR;
	if (isWhiteTile)
	{
		graph = whiteTile;
	}
	else
	{
		graph = floorGraph;
	}

	DirectDrawing::ChangeOBJShader();
	Library::DrawPolygon::GetInstance()->Draw(floorBox, floorPos, Math::Identity(), Vector3::Scale_xyz(1.0f), Color::AddAlphaValue(Color::WHITE, 1.0f), graph);
}

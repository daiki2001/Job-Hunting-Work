#include "BlockType.h"
#include "BlockManager.h"

const float BlockType::BLOCK_SIZE = 1.0f;
const float BlockType::BLOCK_HEIGHT = 1.5f;
const float BlockType::FLOOR_HEIGHT = 0.5f;
const std::string BlockType::blockResourcesDir = "./Resources/Game/Block/";
DrawPolygon* BlockType::draw = nullptr;
int BlockType::floorGraph = FUNCTION_ERROR;
int BlockType::floorObj = FUNCTION_ERROR;
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
	if (switchBlock == FUNCTION_ERROR)
	{
		switchBlock = BlockType::draw->CreateOBJModel("./Resources/Game/Block/Switch.obj", "./Resources/Game/Block/");
	}
	if (blueSwitchBlock == FUNCTION_ERROR)
	{
		blueSwitchBlock = BlockType::draw->CreateOBJModel("./Resources/Game/Block/Switch_blue.obj", "./Resources/Game/Block/");
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
		blockBox = draw->Create3Dbox(Vector3::Scale_xyz(BLOCK_SIZE));
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
			blockBox = draw->Create3Dbox(Vector3::Scale_xyz(BLOCK_SIZE));
		}
	}

	this->rotation = rotation;
	this->scale = scale;
	this->color = color;

	return blockBox;
}

void BlockType::Draw(const Vector3& offset) const
{
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

void BlockType::FloorDraw(const Vector3& offset)
{
	Vector3 floorPos = offset;
	floorPos.z += (BLOCK_HEIGHT + FLOOR_HEIGHT) / 2.0f;
	DirectDrawing::ChangeOBJShader();
	draw->Draw(floorObj, floorPos, Math::Identity(), Vector3::Scale_xyz(1.0f), Color::AddAlphaValue(Color::WHITE, 1.0f), floorGraph);
}

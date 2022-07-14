#include "BlockType.h"

const int BlockType::WIDTH = 32;
const int BlockType::HEIGHT = 32;
const std::wstring BlockType::blockResourcesDir = L"./Resources/Game/Block/";
DrawPolygon* BlockType::draw = nullptr;
int BlockType::BlockBox = Engine::FUNCTION_ERROR;

BlockType::BlockType(const int& typeId, DrawPolygon* const draw) :
	typeId(typeId),
	graph(Engine::FUNCTION_ERROR)
{
	if (this->draw == nullptr)
	{
		this->draw = draw;
	}
}

BlockType::~BlockType()
{
}

int BlockType::Create(const wchar_t* filename)
{
	graph = draw->LoadTextrue((blockResourcesDir + filename).c_str());
	BlockBox = draw->Create3Dbox(1.0f, 1.0f, 1.0f);
	return graph;
}

void BlockType::Draw(const Vector3& pos)
{
	// 'typeId'が0(None)以外
	if (typeId != 0)
	{
		draw->Draw(BlockBox, pos, Math::Identity(), Vector3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), graph);
	}
}

#ifdef _DEBUG
void BlockType::Draw(const Vector3& pos, const DirectX::XMFLOAT4& color)
{
	// 'typeId'が0(None)以外
	if (typeId != 0)
	{
		draw->Draw(BlockBox, pos, Math::Identity(), Vector3(1.0f, 1.0f, 1.0f), color, graph);
	}
}
#endif // _DEBUG


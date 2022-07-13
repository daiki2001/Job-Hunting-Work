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
	draw->Draw(BlockBox, pos, Math::Identity(), Vector3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), graph);
	//draw->DrawTextrue(static_cast<float>(posX), static_cast<float>(posY), WIDTH, HEIGHT, 0, graph, DirectX::XMFLOAT2(0.0f, 0.0f));
}

#ifdef _DEBUG
void BlockType::Draw(const Vector3& pos, const DirectX::XMFLOAT4& color)
{
	draw->Draw(BlockBox, pos, Math::Identity(), Vector3(1.0f, 1.0f, 1.0f), color, graph);
	//draw->DrawTextrue(static_cast<float>(posX), static_cast<float>(posY), WIDTH, HEIGHT, 0, graph, DirectX::XMFLOAT2(0.0f, 0.0f), color);
}
#endif // _DEBUG


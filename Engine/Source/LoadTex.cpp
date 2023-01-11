﻿#include "./Header/LoadTex.h"
#include "./Header/DirectXInit.h"
#include "./Header/Camera.h"

/*DirectXTexのライブラリ情報*/
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_d.lib")
#else // _DEBUG
#pragma comment(lib, "DirectXTex.lib")
#endif // !_DEBUG

TextrueCommon::TextrueCommon() :
	metadata{},
	scratchImg{},
	img{},
	descHeap{}
{
	noneTextrue[0] = { 1.0f, 1.0f, 1.0f, 1.0f };
}

UINT LoadTex::textrueCount = 0;

LoadTex::LoadTex() :
	DirectDrawing(),
	texCommonData{},
	textrueData{},
	spriteCount(0)
{
}

LoadTex::~LoadTex()
{
	DataClear();
}

HRESULT LoadTex::CreateDescriptorHeap()
{
	HRESULT hr = S_FALSE;
	static auto* dev = DirectXInit::GetDevice();
	
	if (isLoadTexInit == true)
	{
		return S_OK;
	}

	isLoadTexInit = true;

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{}; //設定構造体
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //シェーダから見える
	descHeapDesc.NumDescriptors = (UINT)(50000); //テクスチャバッファの数
	// デスクリプタヒープの生成
	hr = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&texCommonData.descHeap));
	
	return hr;
}

int LoadTex::LoadTextrue(const wchar_t* fileName)
{
	using namespace Engine;

	HRESULT hr = S_FALSE;
	static auto* dev = DirectXInit::GetDevice();
	CD3DX12_RESOURCE_DESC texResDesc{};
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体

	if (FAILED(CreateDescriptorHeap()))
	{
		return FUNCTION_ERROR;
	}

	if (fileName == nullptr)
	{
		return FUNCTION_ERROR;
	}

	for (size_t i = 0; i < textrueData.size(); i++)
	{
		if (fileName == textrueData.at(i).name)
		{
			return static_cast<int>(i);
		}
	}

	textrueCount++;

	// WICテクスチャのロード
	LoadFromWICFile(
		fileName,
		DirectX::WIC_FLAGS_IGNORE_SRGB,
		&texCommonData.metadata,
		texCommonData.scratchImg
	);

	texCommonData.img = texCommonData.scratchImg.GetImage(0, 0, 0);

	texResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		texCommonData.metadata.format,
		texCommonData.metadata.width,
		(UINT)texCommonData.metadata.height,
		(UINT16)texCommonData.metadata.arraySize,
		(UINT16)texCommonData.metadata.mipLevels
	);

	textrueData.emplace_back(Textrue{});
	textrueData.back().name = fileName;

	hr = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, //テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&textrueData.back().texbuff)
	);
	if (FAILED(hr))
	{
		return FUNCTION_ERROR;
	}

	// テクスチャバッファにデータ転送
	hr = textrueData.back().texbuff->WriteToSubresource(
		0,
		nullptr,                            //全領域へコピー
		texCommonData.img->pixels,          //元データアドレス
		(UINT)texCommonData.img->rowPitch,  //1ラインサイズ
		(UINT)texCommonData.img->slicePitch //一枚サイズ
	);
	if (FAILED(hr))
	{
		return FUNCTION_ERROR;
	}

	// デスクリプタヒープの先頭ハンドル(CPU)を取得
	textrueData.back().cpuDescHandle =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			texCommonData.descHeap->GetCPUDescriptorHandleForHeapStart(),
			(INT)(textrueData.size() - 1),
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);

	// デスクリプタヒープの先頭ハンドル(GPU)を取得
	textrueData.back().gpuDescHandle =
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			texCommonData.descHeap->GetGPUDescriptorHandleForHeapStart(),
			(INT)(textrueData.size() - 1),
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);

	// シェーダリソースビュー設定
	srvDesc.Format = texCommonData.metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(
		textrueData.back().texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		textrueData.back().cpuDescHandle
	);

	return static_cast<int>(textrueData.size() - 1);
}

void LoadTex::DataClear()
{
	for (size_t i = 0; i < textrueData.size(); i++)
	{
		textrueCount--;
	}

	ContainerClear(textrueData);
	texCommonData = {};

#ifdef _DEBUG
	OutputDebugStringA("'LoadTex'のデータを削除しました。\n");
#endif // _DEBUG
}

int LoadTex::DrawTextureInit()
{
	using namespace Engine;

	int size = 0;

	size = CreateSprite();
	if (size == FUNCTION_ERROR)
	{
		return FUNCTION_ERROR;
	}

	if (isDrawTextrueInit == false)
	{
		isDrawTextrueInit = true;

		if (LoadTextrue() == FUNCTION_ERROR)
		{
			return FUNCTION_ERROR;
		}
	}

	return size;
}

int LoadTex::DrawTextrue(float posX, float posY, float width, float height, float angle,
						 int graphHandle, const DirectX::XMFLOAT2& anchorpoint,
						 const XMFLOAT4& color, int parent)
{
	using namespace DirectX;
	using namespace Engine;

	if ((graphHandle < 0 || (UINT)graphHandle > textrueCount) ||
		(parent < -1 || (parent != -1 && (size_t)parent >= spriteIndex.size())))
	{
		return FUNCTION_ERROR;
	}

	static auto* cmdList = DirectXInit::GetCommandList();
	bool isInit = false;

	if ((size_t)(spriteCount + 1) < spriteIndex.size())
	{
		isInit = true;
	}

	if (isInit == false)
	{
		int size = DrawTextureInit();
		if (size == FUNCTION_ERROR)
		{
			return FUNCTION_ERROR;
		}

		spriteIndex.emplace_back(IndexData{ size, graphHandle });
	}

	if (spriteIndex.size() == 0)
	{
		return FUNCTION_ERROR;
	}

	spriteCount++;
	spriteCount %= spriteIndex.size();

#pragma region GraphicsCommand

	BaseDrawSpriteGraphics();

	IndexData& index = spriteIndex[spriteCount];
	index.textrue = graphHandle;

	if (sprite[index.constant].size.x != width ||
		sprite[index.constant].size.y != height ||
		sprite[index.constant].anchorpoint.x != anchorpoint.x ||
		sprite[index.constant].anchorpoint.y != anchorpoint.y ||
		sprite[index.constant].isTriangle == true)
	{
		sprite[index.constant].size = { width, height };
		sprite[index.constant].anchorpoint = anchorpoint;
		sprite[index.constant].isTriangle = false;

		enum Corner { LB, LT, RB, RT };

		SpriteVertex vert[] = {
			{{}, { 0.0f, 1.0f }},
			{{}, { 0.0f, 0.0f }},
			{{}, { 1.0f, 1.0f }},
			{{}, { 1.0f, 0.0f }},
		};

		float left = (0.0f - sprite[index.constant].anchorpoint.x) * sprite[index.constant].size.x;
		float right = (1.0f - sprite[index.constant].anchorpoint.x) * sprite[index.constant].size.x;
		float top = (0.0f - sprite[index.constant].anchorpoint.y) * sprite[index.constant].size.y;
		float bottom = (1.0f - sprite[index.constant].anchorpoint.y) * sprite[index.constant].size.y;

		vert[LB].pos = { left, bottom, 0.0f };
		vert[LT].pos = { left, top, 0.0f };
		vert[RB].pos = { right, bottom, 0.0f };
		vert[RT].pos = { right, top, 0.0f };

		// 頂点バッファへのデータ転送
		SpriteVertex* vertexMap = nullptr;
		sprite[index.constant].vertBuff->Map(0, nullptr, (void**)&vertexMap);
		memcpy(vertexMap, vert, sizeof(vert));
		sprite[index.constant].vertBuff->Unmap(0, nullptr);
	}

	sprite[index.constant].color = color;
	sprite[index.constant].pos = { posX, posY, 0 };
	sprite[index.constant].rotation = angle;

	sprite[index.constant].matWorld = Math::Identity();
	sprite[index.constant].matWorld *= Math::rotateZ(sprite[index.constant].rotation);
	sprite[index.constant].matWorld *= Math::translate(sprite[index.constant].pos);

	if (parent != -1)
	{
		sprite[index.constant].matWorld *= sprite[parent].matWorld;
	}

	SpriteConstBufferData* constMap = nullptr;
	HRESULT hr = sprite[index.constant].constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(hr))
	{
		constMap->color = sprite[index.constant].color;
		constMap->mat = sprite[index.constant].matWorld *
			Camera::matProjection[Camera::Projection::ORTHOGRAPHIC];
		sprite[index.constant].constBuff->Unmap(0, nullptr);
	}

	// デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeaps[] = { texCommonData.descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, sprite[index.constant].constBuff->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootDescriptorTable(1, textrueData[index.textrue].gpuDescHandle);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &sprite[index.constant].vbView);
	// 描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);

#pragma endregion //GraphicsCommand

	return index.constant;
}

int LoadTex::DrawCutTextrue(float posX, float posY, float width, float height,
							const DirectX::XMFLOAT2& texPos, const DirectX::XMFLOAT2& texSize, float angle,
							int graphHandle, const DirectX::XMFLOAT2& anchorpoint, const XMFLOAT4& color, int parent)
{
	using namespace DirectX;
	using namespace Engine;

	if ((graphHandle < 0 || (UINT)graphHandle > textrueCount) ||
		(parent < -1 && (size_t)parent >= spriteIndex.size()))
	{
		return FUNCTION_ERROR;
	}

	static auto* cmdList = DirectXInit::GetCommandList();
	bool isInit = false;

	if ((size_t)(spriteCount + 1) < spriteIndex.size())
	{
		isInit = true;
	}

	if (isInit == false)
	{
		int size = DrawTextureInit();
		if (size == FUNCTION_ERROR)
		{
			return FUNCTION_ERROR;
		}

		spriteIndex.emplace_back(IndexData{ size, graphHandle });
	}

	if (spriteIndex.size() == 0)
	{
		return FUNCTION_ERROR;
	}

	spriteCount++;
	spriteCount %= spriteIndex.size();

#pragma region GraphicsCommand

	BaseDrawSpriteGraphics();

	IndexData& index = spriteIndex[spriteCount];
	index.textrue = graphHandle;

	if (sprite[index.constant].size.x != width ||
		sprite[index.constant].size.y != height ||
		sprite[index.constant].texLeftTop.x != texPos.x ||
		sprite[index.constant].texLeftTop.y != texPos.y ||
		sprite[index.constant].texSize.x != texSize.x ||
		sprite[index.constant].texSize.y != texSize.y ||
		sprite[index.constant].anchorpoint.x != anchorpoint.x ||
		sprite[index.constant].anchorpoint.y != anchorpoint.y ||
		sprite[index.constant].isTriangle == true)
	{
		sprite[index.constant].size = { width, height };
		sprite[index.constant].texLeftTop = texPos;
		sprite[index.constant].texSize = texSize;
		sprite[index.constant].anchorpoint = anchorpoint;
		sprite[index.constant].isTriangle = false;

		enum Corner { LB, LT, RB, RT };

		static SpriteVertex vert[] = {
			{{}, { 0.0f, 1.0f }},
			{{}, { 0.0f, 0.0f }},
			{{}, { 1.0f, 1.0f }},
			{{}, { 1.0f, 0.0f }},
		};

		float left = (0.0f - sprite[index.constant].anchorpoint.x) * sprite[index.constant].size.x;
		float right = (1.0f - sprite[index.constant].anchorpoint.x) * sprite[index.constant].size.x;
		float top = (0.0f - sprite[index.constant].anchorpoint.y) * sprite[index.constant].size.y;
		float bottom = (1.0f - sprite[index.constant].anchorpoint.y) * sprite[index.constant].size.y;

		vert[LB].pos = { left, bottom, 0.0f };
		vert[LT].pos = { left, top, 0.0f };
		vert[RB].pos = { right, bottom, 0.0f };
		vert[RT].pos = { right, top, 0.0f };

		// テクスチャデータ取得
		D3D12_RESOURCE_DESC resDesc = textrueData[index.textrue].texbuff->GetDesc();

		float texLeft = sprite[index.constant].texLeftTop.x / resDesc.Width;
		float texRight = (sprite[index.constant].texLeftTop.x + sprite[index.constant].texSize.x) / resDesc.Width;
		float texTop = sprite[index.constant].texLeftTop.y / resDesc.Height;
		float texBottom = (sprite[index.constant].texLeftTop.y + sprite[index.constant].texSize.y) / resDesc.Height;

		vert[LB].uv = { texLeft, texBottom };
		vert[LT].uv = { texLeft, texTop };
		vert[RB].uv = { texRight, texBottom };
		vert[RT].uv = { texRight, texTop };

		// 頂点バッファへのデータ転送
		SpriteVertex* vertexMap = nullptr;
		sprite[index.constant].vertBuff->Map(0, nullptr, (void**)&vertexMap);
		memcpy(vertexMap, vert, sizeof(vert));
		sprite[index.constant].vertBuff->Unmap(0, nullptr);
	}

	sprite[index.constant].color = color;
	sprite[index.constant].pos = { posX, posY, 0 };
	sprite[index.constant].rotation = angle;

	sprite[index.constant].matWorld = Math::Identity();
	sprite[index.constant].matWorld *= Math::rotateZ(sprite[index.constant].rotation);
	sprite[index.constant].matWorld *= Math::translate(sprite[index.constant].pos);

	if (parent != -1)
	{
		sprite[index.constant].matWorld *= sprite[parent].matWorld;
	}

	SpriteConstBufferData* constMap = nullptr;
	sprite[index.constant].constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = sprite[index.constant].color;
	constMap->mat = sprite[index.constant].matWorld *
		Camera::matProjection[Camera::Projection::ORTHOGRAPHIC];
	sprite[index.constant].constBuff->Unmap(0, nullptr);

	// デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeaps[] = { texCommonData.descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, sprite[index.constant].constBuff->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootDescriptorTable(1, textrueData[index.textrue].gpuDescHandle);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &sprite[index.constant].vbView);
	// 描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);

#pragma endregion //GraphicsCommand

	return index.constant;
}

int LoadTex::Draw2DTriangle(float posX, float posY, float width, float height, float angle, int graphHandle, const DirectX::XMFLOAT2& anchorpoint, const XMFLOAT4& color, int parent)
{
	using namespace DirectX;
	using namespace Engine;

	if ((graphHandle < 0 || (UINT)graphHandle > textrueCount) ||
		(parent < -1 || (parent != -1 && (size_t)parent >= spriteIndex.size())))
	{
		return FUNCTION_ERROR;
	}

	static auto* cmdList = DirectXInit::GetCommandList();
	bool isInit = false;

	if ((size_t)(spriteCount + 1) < spriteIndex.size())
	{
		isInit = true;
	}

	if (isInit == false)
	{
		int size = DrawTextureInit();
		if (size == FUNCTION_ERROR)
		{
			return FUNCTION_ERROR;
		}

		spriteIndex.emplace_back(IndexData{ size, graphHandle });
	}

	if (spriteIndex.size() == 0)
	{
		return FUNCTION_ERROR;
	}

	spriteCount++;
	spriteCount %= spriteIndex.size();

#pragma region GraphicsCommand

	BaseDrawSpriteGraphics();

	IndexData& index = spriteIndex[spriteCount];
	index.textrue = graphHandle;

	if (sprite[index.constant].size.x != width ||
		sprite[index.constant].size.y != height ||
		sprite[index.constant].anchorpoint.x != anchorpoint.x ||
		sprite[index.constant].anchorpoint.y != anchorpoint.y ||
		sprite[index.constant].isTriangle == false)
	{
		sprite[index.constant].size = { width, height };
		sprite[index.constant].anchorpoint = anchorpoint;
		sprite[index.constant].isTriangle = true;

		enum Corner { LEFT, TOP, BOTTOM };

		SpriteVertex vert[] = {
			{{}, { 0.0f, 0.5f }},
			{{}, { 1.0f, 0.0f }},
			{{}, { 1.0f, 1.0f }},
		};

		float left = (0.0f - sprite[index.constant].anchorpoint.x) * sprite[index.constant].size.x;
		float centerX = (0.5f - sprite[index.constant].anchorpoint.x) * sprite[index.constant].size.x;
		float right = (1.0f - sprite[index.constant].anchorpoint.x) * sprite[index.constant].size.x;
		float top = (0.0f - sprite[index.constant].anchorpoint.y) * sprite[index.constant].size.y;
		float bottom = (1.0f - sprite[index.constant].anchorpoint.y) * sprite[index.constant].size.y;

		vert[LEFT].pos = { left, centerX, 0.0f };
		vert[TOP].pos = { right, top, 0.0f };
		vert[BOTTOM].pos = { right, bottom, 0.0f };

		// 頂点バッファへのデータ転送
		SpriteVertex* vertexMap = nullptr;
		sprite[index.constant].vertBuff->Map(0, nullptr, (void**)&vertexMap);
		memcpy(vertexMap, vert, sizeof(vert));
		sprite[index.constant].vertBuff->Unmap(0, nullptr);
	}

	sprite[index.constant].color = color;
	sprite[index.constant].pos = { posX, posY, 0 };
	sprite[index.constant].rotation = angle;

	sprite[index.constant].matWorld = Math::Identity();
	sprite[index.constant].matWorld *= Math::rotateZ(sprite[index.constant].rotation);
	sprite[index.constant].matWorld *= Math::translate(sprite[index.constant].pos);

	if (parent != -1)
	{
		sprite[index.constant].matWorld *= sprite[parent].matWorld;
	}

	SpriteConstBufferData* constMap = nullptr;
	HRESULT hr = sprite[index.constant].constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(hr))
	{
		constMap->color = sprite[index.constant].color;
		constMap->mat = sprite[index.constant].matWorld *
			Camera::matProjection[Camera::Projection::ORTHOGRAPHIC];
		sprite[index.constant].constBuff->Unmap(0, nullptr);
	}

	// デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeaps[] = { texCommonData.descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, sprite[index.constant].constBuff->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootDescriptorTable(1, textrueData[index.textrue].gpuDescHandle);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &sprite[index.constant].vbView);
	// 描画コマンド
	cmdList->DrawInstanced(3, 1, 0, 0);

#pragma endregion //GraphicsCommand

	return index.constant;
}

void LoadTex::LoopEnd()
{
	spriteCount = -1;
}

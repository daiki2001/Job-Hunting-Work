﻿#pragma once
#include "DirectDrawing.h"
#include <DirectXTex.h>
#include <d3dx12.h>
#include <string>

// テクスチャ全体のデータをまとめた構造体
struct TextrueCommon
{
	DirectX::XMFLOAT4 noneTextrue[1]; //画像無し用
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage scratchImg;
	const DirectX::Image* img;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap; //srv専用のディスクリプタヒープ

	TextrueCommon();
};

// テクスチャ一枚分のデータをまとめた構造体
struct Textrue
{
	std::wstring name;
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff; //テクスチャバッファ

	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandle; //CPU側のハンドル
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle; //GPU側のハンドル
};

class LoadTex : public DirectDrawing
{
	/*namespace名省略*/
protected:
	using XMFLOAT4 = DirectX::XMFLOAT4;

	/*メンバ変数*/
protected:
	static UINT textrueCount;

	/*メンバ変数*/
protected:
	vector<Textrue> textrueData;
	TextrueCommon texCommonData;
private:
	UINT spriteCount;

	// 関数の初期化フラグ
	bool isLoadTexInit = false;
	bool isDrawTextrueInit = false;

	/*メンバ関数*/
public:
	LoadTex();
	~LoadTex();

	// ディスクリプタヒープの生成
	HRESULT CreateDescriptorHeap();

	// テクスチャバッファの生成
	int LoadTextrue(const wchar_t* fileName = nullptr);

	// スプライトの描画
	int DrawTextrue(float posX, float posY, float width, float height, float angle = 0,
					int graphHandle = 0, const DirectX::XMFLOAT2& anchorpoint = { 0.5f, 0.5f },
					const XMFLOAT4& color = { 1, 1, 1, 1 }, int parent = -1);

	// スプライトの切り出し描画
	int DrawCutTextrue(float posX, float posY, float width, float height,
					   const DirectX::XMFLOAT2& texPos, const DirectX::XMFLOAT2& texSize, float angle = 0,
					   int graphHandle = 0, const DirectX::XMFLOAT2& anchorpoint = { 0.5f, 0.5f },
					   const XMFLOAT4& color = { 1, 1, 1, 1 }, int parent = -1);

	// 内部の初期化用関数
	void LoopEnd();

	int DrawTextureInit();
private:
	// データの消去
	void DataClear();
};

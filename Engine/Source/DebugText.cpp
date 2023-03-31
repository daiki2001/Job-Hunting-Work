﻿#include "./Header/DebugText.h"
#include "./Header/DirectXInit.h"
#include "./Header/Camera.h"
#include "./Header/Parameter.h"

/*static変数の初期化*/
int DebugText::fontTex = FUNCTION_ERROR;
UINT64 DebugText::fontTexWidth = {};
UINT DebugText::fontTexHeight = {};

DebugText::DebugText() :
	LoadTex(),
	fontIndex{},
	charCount(-1)
{
	Init();
}

DebugText::~DebugText()
{
	DataClear();
}

HRESULT DebugText::Init()
{
	HRESULT hr = S_FALSE;

	if (fontTex == FUNCTION_ERROR)
	{
		// デバッグテキスト用のテキスト画像の読み込み
		fontTex = LoadTextrue((StringToWString(engineResourcesDir) + L"DebugTextFont.png").c_str());
		if (fontTex == FUNCTION_ERROR)
		{
			return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
		}

		// テクスチャデータ取得
		D3D12_RESOURCE_DESC resDesc = textrueData[fontTex].texbuff->GetDesc();

		fontTexWidth = resDesc.Width;
		fontTexHeight = resDesc.Height;
	}

	return S_OK;
}

HRESULT DebugText::DrawStringInit()
{
	HRESULT hr = S_FALSE;
	static bool isInit = false;

	// 初期化されていたらreturnする
	if (isInit == true)
	{
		return S_OK;
	}
	else
	{
		isInit = true;
		fontIndex.push_back({});

		fontIndex.back() = CreateSprite();
		if (fontIndex.back() == FUNCTION_ERROR)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

HRESULT DebugText::DrawString(float posX, float posY, float fontScale,
							  const XMFLOAT4& color1, const XMFLOAT4& color2,
							  const char* text, ...)
{
	using namespace DirectX;

	HRESULT hr = S_FALSE;
	static auto* cmdList = DirectXInit::GetCommandList();
	static bool isDrawString = false;
	static char* string = nullptr;
	static char* numberString = static_cast<char*>(malloc(sizeof(char) * 21));
	static char character = '\0';
	static bool formatFlag = false;
	va_list args;
	va_start(args, text);

	if (isDrawString == false)
	{
		isDrawString = true;

		hr = DrawStringInit();
		if (FAILED(hr))
		{
			return hr;
		}
	}

#pragma region GraphicsCommand

	BaseDrawSpriteGraphics();

	XMFLOAT2 pixel = { 1.0f * fontScale / fontTexWidth, 1.0f * fontScale / fontTexHeight };

	// デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeaps[] = { texCommonData.descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	for (size_t i = 0, j = 0, k = 0; text[j] != '\0'; i++)
	{
		bool isInit = false;

		if (text[j] == '%')
		{
			formatFlag = true;
		}

		if (formatFlag == true)
		{
			if (text[j + 1] == '\0')
			{
				break;
			}
			if (text[j + 1] == '%')
			{
				j++;
				character = '%';
				formatFlag = false;
			}
			else if (text[j + 1] == 'c')
			{
				j++;
				character = va_arg(args, char);
				formatFlag = false;
			}
			else
			{
				if (string == nullptr)
				{
					k = 0;
					int tempI;
					double tempF;

					switch (text[j + 1])
					{
					case 'd':
						tempI = va_arg(args, int);
						string = numberString;
						sprintf_s(static_cast<char*>(string), 21, "%d", tempI);
						break;
					case 'f':
						tempF = va_arg(args, double);
						string = numberString;
						sprintf_s(static_cast<char*>(string), 21, "%f", tempF);
						break;
					case 's':
						string = va_arg(args, char*);
						break;
					default:
						break;
					}
				}

				if (string[k] == '\0')
				{
					j += 2;
					i--;
					string = nullptr;
					formatFlag = false;
					continue;
				}
				else
				{
					character = string[k];
				}
			}
		}
		else
		{
			character = text[j];
		}

		if ((size_t)(charCount + 1) < fontIndex.size())
		{
			isInit = true;
		}

		if (isInit == false)
		{
			fontIndex.push_back(CreateSprite());
			if (fontIndex.back() == FUNCTION_ERROR)
			{
				return S_FALSE;
			}
		}

		charCount++;

		sprite[fontIndex[charCount]].size = { fontWidth * fontScale, fontHeight * fontScale };
		sprite[fontIndex[charCount]].texLeftTop.x = (character % fontLineCount) * static_cast<float>(fontWidth);
		sprite[fontIndex[charCount]].texLeftTop.y = static_cast<float>(character / fontLineCount) * static_cast<float>(fontHeight);
		sprite[fontIndex[charCount]].texSize = { fontWidth, fontHeight };

		DrawTextrue(posX + fontWidth * fontScale * i + pixel.x, posY + pixel.y, fontWidth * fontScale, fontHeight * fontScale,
					0.0f, Parameter::Get("white1x1"), DirectX::XMFLOAT2(0.0f, 0.0f), color2);

		enum Corner { LB, LT, RB, RT };

		SpriteVertex vert[4] = {};

		float left = 0.0f;
		float right = sprite[fontIndex[charCount]].size.x;
		float top = 0.0f;
		float bottom = sprite[fontIndex[charCount]].size.y;

		vert[LB].pos = { left,  bottom, 0.0f };
		vert[LT].pos = { left,     top, 0.0f };
		vert[RB].pos = { right, bottom, 0.0f };
		vert[RT].pos = { right,    top, 0.0f };

		float texLeft =
			sprite[fontIndex[charCount]].texLeftTop.x / fontTexWidth;
		float texRight =
			(sprite[fontIndex[charCount]].texLeftTop.x + sprite[fontIndex[charCount]].texSize.x - 1) / fontTexWidth;
		float texTop =
			sprite[fontIndex[charCount]].texLeftTop.y / fontTexHeight;
		float texBottom =
			(sprite[fontIndex[charCount]].texLeftTop.y + sprite[fontIndex[charCount]].texSize.y - 1) / fontTexHeight;

		vert[LB].uv = { texLeft,  texBottom };
		vert[LT].uv = { texLeft,     texTop };
		vert[RB].uv = { texRight, texBottom };
		vert[RT].uv = { texRight,    texTop };

		// 頂点バッファへのデータ転送
		SpriteVertex* vertexMap = nullptr;
		hr = sprite[fontIndex[charCount]].vertBuff->Map(0, nullptr, (void**)&vertexMap);
		memcpy(vertexMap, vert, sizeof(vert));
		sprite[fontIndex[charCount]].vertBuff->Unmap(0, nullptr);

		sprite[fontIndex[charCount]].color = color1;
		sprite[fontIndex[charCount]].pos = { posX + fontWidth * fontScale * i + pixel.x, posY + pixel.y, 0 };
		sprite[fontIndex[charCount]].rotation = 0;

		sprite[fontIndex[charCount]].matWorld = XMMatrixIdentity();
		sprite[fontIndex[charCount]].matWorld *=
			XMMatrixRotationZ(XMConvertToRadians(sprite[fontIndex[charCount]].rotation));
		sprite[fontIndex[charCount]].matWorld *= XMMatrixTranslation(
			sprite[fontIndex[charCount]].pos.x,
			sprite[fontIndex[charCount]].pos.y,
			sprite[fontIndex[charCount]].pos.z);

		SpriteConstBufferData* constMap = nullptr;
		hr = sprite[fontIndex[charCount]].constBuff->Map(0, nullptr, (void**)&constMap);
		constMap->color = sprite[fontIndex[charCount]].color;
		constMap->mat = sprite[fontIndex[charCount]].matWorld *
			Camera::matProjection[Camera::Projection::ORTHOGRAPHIC];
		sprite[fontIndex[charCount]].constBuff->Unmap(0, nullptr);

		// 定数バッファビューをセット
		cmdList->SetGraphicsRootConstantBufferView(0, sprite[fontIndex[charCount]].constBuff->GetGPUVirtualAddress());
		// シェーダーリソースビューをセット
		cmdList->SetGraphicsRootDescriptorTable(1, textrueData[fontTex].gpuDescHandle);

		// 頂点バッファの設定
		cmdList->IASetVertexBuffers(0, 1, &sprite[fontIndex[charCount]].vbView);
		// 描画コマンド
		cmdList->DrawInstanced(4, 1, 0, 0);

		if (formatFlag == true)
		{
			k++;
		}
		else
		{
			j++;
		}
	}

#pragma endregion

	va_end(args);
	return S_OK;
}

void DebugText::TextLoopEnd()
{
	LoopEnd();

	charCount = -1;
}

void DebugText::DataClear()
{
	ContainerClear(fontIndex);
	fontTex = FUNCTION_ERROR;

#ifdef _DEBUG
	OutputDebugStringA("'DebugText'のデータを削除しました。\n");
#endif // _DEBUG
}

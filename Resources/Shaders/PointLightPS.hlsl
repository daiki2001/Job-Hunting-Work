#include "SpriteData.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 output = color;
	float2 lig = input.uv - float2(0.5f, 0.5f);
	return output * sqrt((lig.x * lig.x) + (lig.y * lig.y));
}
#include "PointLightData.hlsli"

struct VSInput
{
    float3 pos : POSITION; //システム用頂点座標
    float2 uv : TEXCOORD0; //uv座標
    float3 normal : NORMAL; //法線
};

VSOutput main(VSInput input)
{
	VSOutput output;
    output.svpos = mul(float4(input.pos, 1.0f), worldViewProj);
    output.uv = input.uv;
    output.worldPos = mul(float4(input.pos, 1.0f), world).xyz;
    output.normal = mul(float4(input.normal, 0.0f), world).xyz;
    output.lightVec = lightPosition - output.worldPos;
	return output;
}
#include "PointLightData.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      //0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float3 N = normalize(input.normal);
    float D = dot(-normalize(input.lightVec), N);
    float3 diffuse = tex.Sample(smp, input.uv).rgb * lightColor * saturate(D) * (1.0f / (lightRadius * lightRadius));
    float3 ambient = ambientColor * tex.Sample(smp, input.uv).rgb;
    return float4(diffuse + ambient, 1.0f);
}
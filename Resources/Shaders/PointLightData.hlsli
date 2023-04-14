cbuffer cbuff0 : register(b0)
{
    matrix worldViewProj;
    matrix world;
    float3 lightPosition;
    float lightRadius;
    float3 lightColor;
    float3 ambientColor;

	// float4 color; //色 (RGBA)
	// matrix mat;   //3D変換行列
}

struct VSOutput
{
	float4 svpos  : SV_POSITION; //システム用頂点座標
	float2 uv     : TEXCOORD;    //uv座標
    float3 worldPos : TEXCOORD1; //座標
    float3 normal : TEXCOORD2; //法線
    float3 lightVec : TEXCOORD3; //ライトの方向ベクトル
};
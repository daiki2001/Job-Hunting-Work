#include "FBX.hlsli"

struct PSOutput
{
	float4 target0 : SV_Target0;
	float4 target1 : SV_Target1;
};

PSOutput main(VSOutput input)
{
	PSOutput output;

	output.target0 = color;
	output.target1 = color;
	return output;
}
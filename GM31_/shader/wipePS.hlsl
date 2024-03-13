
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse.rgb = float3(0.0f, 0.0f, 0.0f);

	float dissolveValue = g_Texture.Sample(g_SamplerState, In.TexCoord);

	//ワイプ
	//outDiffuse.a = step(dissolveThreshold, dissolveValue);

	//ワイプα値
	float threshold = parameter.x * (1.0f + parameter.y) - parameter.y;

	float rate = saturate((dissolveValue - threshold) / parameter.y);

	outDiffuse.a = rate;
}
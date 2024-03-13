
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	if (Material.TextureEnable) {
		outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
		outDiffuse *= In.Diffuse;
	}
	else {
		outDiffuse = In.Diffuse;
	}


	if (color.a == 1.0f) {
		outDiffuse.rgb *= color.xyz;
		outDiffuse.a *= color.a;
	}
}

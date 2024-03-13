
#include "common.hlsl"

static const float pi = 3.1415926535f;

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float2 uv = In.TexCoord - 0.5;

	float angle = atan2(uv.x, uv.y);
	angle += pi;

	float4 backColor = float4(0.3f, 0.3f, 0.3f, 1.0f);

	float gage = frac(angle * 0.5 / pi) * parameter.x;
	outDiffuse.rgb += backColor.rgb * step(parameter.y, gage);
	outDiffuse.rgb += color.rgb * (1.0f - step(parameter.y, gage));
	//outDiffuse.rgb += param.diffColor.rgb * (1.0f - step(parameter.z, gage)) * step(parameter.x, gage);
	float dist = length(uv) / 0.5;
	outDiffuse.a = (1.0 - step(1.0, dist)) * step(0.5, dist);

}

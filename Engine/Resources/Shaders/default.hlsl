#include "params.hlsl"

VERTEX_OUT VS_Main(VERTEX_IN vin)
{
	VERTEX_OUT vout = (VERTEX_OUT)0.0f;
	vout.position = vin.position;
	vout.color = vin.color;
	vout.uv = vin.uv;
	vout.position_ndc = mul(float4(vin.position, 1.0f), _cb_offset);
	
	return vout;
}

float4 PS_Main(VERTEX_OUT vin) : SV_TARGET
{
	float4 texture_color = _texture_0.Sample(_samp_anisotropic_wrap_16, vin.uv);
	float4 color = saturate(_texture_1.Sample(_samp_anisotropic_wrap_16, vin.uv) + 0.25f);
	color.a = 1.0f;

	if (color.r > 0.45f)
		texture_color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	//return vin.color;
	return texture_color;
}

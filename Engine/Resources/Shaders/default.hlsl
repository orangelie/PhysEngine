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
	float4 color = _texture_0.Sample(_samp_anisotropic_wrap_16, vin.uv);

	//return vin.color;
	return color;
}

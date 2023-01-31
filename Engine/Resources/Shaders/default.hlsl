#include "params.hlsl"

float3 NormalMapping(float3 normalSample, float3 normal, float3 tangent)
{
	float3 normalT = (2.0f * normalSample) - 1.0f;

	float3 N = normalize(normal);
	float3 T = normalize(tangent - dot(tangent, N) * N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);
	return mul(normalT, TBN);
}

VERTEX_OUT VS_Main(VERTEX_IN vin)
{
	VERTEX_OUT vout = (VERTEX_OUT)0.0f;
	vout.position = mul(float4(vin.position, 1.0f), bWorld);
	vout.normal = mul(float4(vin.normal, 1.0f), bWorld);
	vout.tangent = mul(float4(vin.tangent, 1.0f), bWorld);
	vout.uv = vin.uv;
	vout.position_ndc = mul(float4(vin.position, 1.0f), bWorldViewProj);
	
	return vout;
}

float4 PS_Main(VERTEX_OUT vin) : SV_TARGET
{
	float4 texture_color = _texture_0.Sample(_samp_anisotropic_wrap_16, vin.uv);
	float4 normal_map = _texture_1.Sample(_samp_anisotropic_wrap_16, vin.uv);
	vin.normal = NormalMapping(normal_map.rgb, vin.normal, vin.tangent);



	return texture_color;
}

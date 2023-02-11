#ifndef __DEFAULT_HLSL__
#define __DEFAULT_HLSL__

#include "params.hlsl"
#include "lighting.hlsl"

struct VERTEX_IN
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXCOORD;
};

struct VERTEX_OUT
{
	float4 position_ndc : SV_POSITION;
	float2 uv : TEXCOORD;
};

VERTEX_OUT VS_Main(VERTEX_IN vin)
{
	VERTEX_OUT vout = (VERTEX_OUT)0.0f;
	float4 posW = mul(float4(vin.position, 1.0f), gWorld);
	vout.uv = vin.uv;
	vout.position_ndc = mul(posW, gView);
	vout.position_ndc = mul(vout.position_ndc, gProj);
	
	return vout;
}

float4 PS_Main(VERTEX_OUT vin) : SV_TARGET
{
	return _texture_0.Sample(_samp_point_wrap, vin.uv);
}

#endif
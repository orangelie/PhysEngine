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
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXCOORD;
};

VERTEX_OUT VS_Main(VERTEX_IN vin)
{
	VERTEX_OUT vout = (VERTEX_OUT)0.0f;
	float4 posW = mul(float4(vin.position, 1.0f), gWorld);

	vout.position = posW.xyz;
	vout.normal = mul(vin.normal, (float3x3)gWorld);
	vout.tangent = mul(vin.tangent, (float3x3)gWorld);
	vout.uv = vin.uv;
	vout.position_ndc = mul(posW, gView);
	vout.position_ndc = mul(vout.position_ndc, gProj);
	
	return vout;
}

float4 PS_Main(VERTEX_OUT vin) : SV_TARGET
{
	float4 diffuse_map = _texture_0.Sample(_samp_anisotropic_wrap_16, vin.uv) * gDiffuseAlbedo;
	float4 normal_map = _texture_1.Sample(_samp_anisotropic_wrap_16, vin.uv);
	float rough_map = _texture_2.Sample(_samp_anisotropic_wrap_16, vin.uv).r;
	vin.normal = NormalMapping(normal_map.rgb, vin.normal, vin.tangent);
	vin.normal = normalize(vin.normal);

	// NOTE
	float roughness = rough_map;

	float3 toEyeW = normalize(gEyePos - vin.position);

	float4 ambient = gAmbientLight * diffuse_map;

	const float shininess = 1.0f - roughness;
	Material mat = { diffuse_map, gFresnelR0, shininess };
	float3 shadowFactor = 1.0f;
	float4 directLight = ComputeLighting(gLights, mat, vin.position,
		vin.normal, toEyeW, shadowFactor);

	float4 litColor = ambient + directLight;

	litColor.a = gDiffuseAlbedo.a;

	return litColor;
}

#endif
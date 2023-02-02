#ifndef __PARAMS_HLSL__
#define __PARAMS_HLSL__

#define GLOBAL_MAX_LIGHT (16)

struct Light
{
	float3 Strength;
	float FalloffStart;
	float3 Direction;
	float FalloffEnd;
	float3 Position;
	float SpotPower;
};

struct Material
{
	float4 DiffuseAlbedo;
	float3 FresnelR0;
	float Shininess;
};

cbuffer cbPerTransform : register(b0)
{
	row_major matrix gView;
	row_major matrix gProj;
	row_major matrix gWorld;
	float3 gEyePos;
	float __g_b0_pad1__;
};

cbuffer cbPerLight : register(b1)
{
	float4 gAmbientLight;
	float4 gDiffuseAlbedo;
	float3 gFresnelR0;
	float gRoughness;
	uint gDirectionalLightCount;
	uint gPointLightCount;
	uint gSpotLightCount;
	uint __g_b1_Pad1__;
	Light gLights[GLOBAL_MAX_LIGHT];
};

Texture2D _texture_0 : register(t0);
Texture2D _texture_1 : register(t1);
Texture2D _texture_2 : register(t2);
Texture2D _texture_3 : register(t3);
Texture2D _texture_4 : register(t4);
Texture2D _texture_5 : register(t5);

SamplerState _samp_point_wrap			: register(s0);
SamplerState _samp_point_clamp			: register(s1);
SamplerState _samp_linear_wrap			: register(s2);
SamplerState _samp_linear_clamp			: register(s3);
SamplerState _samp_anisotropic_wrap		: register(s4);
SamplerState _samp_anisotropic_clamp	: register(s5);
SamplerState _samp_anisotropic_wrap_16	: register(s6);

float3 NormalMapping(float3 normalSample, float3 normal, float3 tangent)
{
	float3 normalT = (2.0f * normalSample) - 1.0f;

	float3 N = normalize(normal);
	float3 T = normalize(tangent - dot(tangent, N) * N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);
	return mul(normalT, TBN);
}

#endif
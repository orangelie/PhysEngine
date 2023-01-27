#ifndef __PARAMS_HEADER__
#define __PARAMS_HEADER__

cbuffer cbPerTransform : register(b0)
{
	row_major matrix _cb_offset;
};

struct VERTEX_IN
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

struct VERTEX_OUT
{
	float4 position_ndc : SV_POSITION;
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

Texture2D _texture_0 : register(t0);
Texture2D _texture_1 : register(t1);
Texture2D _texture_2 : register(t2);
Texture2D _texture_3 : register(t3);
Texture2D _texture_4 : register(t4);

SamplerState _samp_point_wrap			: register(s0);
SamplerState _samp_point_clamp			: register(s1);
SamplerState _samp_linear_wrap			: register(s2);
SamplerState _samp_linear_clamp			: register(s3);
SamplerState _samp_anisotropic_wrap		: register(s4);
SamplerState _samp_anisotropic_clamp	: register(s5);
SamplerState _samp_anisotropic_wrap_16	: register(s6);

#endif
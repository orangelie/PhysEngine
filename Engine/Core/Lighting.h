#pragma once

#define GLOBAL_LIGHT_NUM (16)

struct Light
{
	Vector3 Strength;
	float FalloffStart;
	Vector3 Direction;
	float FalloffEnd;
	Vector3 Position;
	float SpotPower;
};

struct LightPass
{
	Vector4 AmbientLight;
	Vector4 DiffuseAlbedo;
	Vector3 R0;
	float Roughness;
	uint32 DirectionalLightCount;
	uint32 PointLightCount;
	uint32 SpotLightCount;
	uint32 __Pad1__;
	Light Lights[GLOBAL_LIGHT_NUM];
};

class Lighting
{
public:
	Lighting() {}
	virtual ~Lighting() {}

	void Init(const LightPass& lightPass);
	void Update();

private:
	LightPass _lightPass = {};

};


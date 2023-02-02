#include "pch.h"
#include "Lighting.h"
#include "Engine.h"

void Lighting::Init(const LightPass& lightPass)
{
	_lightPass = lightPass;
}

void Lighting::Update()
{
	GEngine->GetFrameBuffering()->GetCurrentCbvBuffer()
		[(uint32)CONSTANT_BUFFER_INDEX::Light]->PushGraphicsData(&_lightPass, sizeof(LightPass));
}

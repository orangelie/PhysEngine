#include "pch.h"
#include "GameObject.h"
#include "FrameBuffering.h"
#include "Engine.h"

void GameObject::Update(Matrix ViewProj)
{
	TransformPass tmpTranformPass = _tranformPass;
	tmpTranformPass.worldViewProj *= ViewProj;
	GEngine->GetFrameBuffering()->GetCurrentCbvBuffer()[(uint32)CONSTANT_BUFFER_INDEX::Transform]->PushGraphicsData(&tmpTranformPass, sizeof(TransformPass));
	
	GEngine->GetFrameBuffering()->GetCurrentGraphicsDescTable()->SetSRV(GEngine->GetTexture()->GetSrvHandle(), SRV_REGISTER::t0);
	GEngine->GetFrameBuffering()->GetCurrentGraphicsDescTable()->SetSRV(GEngine->GetTextureNormal()->GetSrvHandle(), SRV_REGISTER::t1);
	GEngine->GetFrameBuffering()->GetCurrentGraphicsDescTable()->CommitTable();

	_material->Update();
}

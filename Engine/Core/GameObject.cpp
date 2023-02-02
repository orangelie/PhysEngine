#include "pch.h"
#include "GameObject.h"
#include "FrameBuffering.h"
#include "Engine.h"
#include "Lighting.h"

void GameObject::Update(Matrix View, Matrix Proj, Vector3 EyePos)
{
	TransformPass tmpTranformPass = _tranformPass;
	tmpTranformPass.View = View;
	tmpTranformPass.Proj = Proj;
	tmpTranformPass.EyePos = EyePos;

	GEngine->GetFrameBuffering()->GetCurrentCbvBuffer()[(uint32)CONSTANT_BUFFER_INDEX::Transform]->PushGraphicsData(&tmpTranformPass, sizeof(TransformPass));

	GEngine->GetFrameBuffering()->GetCurrentGraphicsDescTable()->SetSRV(GEngine->GetTextureDiffuse()->GetSrvHandle(), SRV_REGISTER::t0);
	GEngine->GetFrameBuffering()->GetCurrentGraphicsDescTable()->SetSRV(GEngine->GetTextureNormal()->GetSrvHandle(), SRV_REGISTER::t1);
	GEngine->GetFrameBuffering()->GetCurrentGraphicsDescTable()->SetSRV(GEngine->GetTextureRough()->GetSrvHandle(), SRV_REGISTER::t2);

	_material->Update();
}

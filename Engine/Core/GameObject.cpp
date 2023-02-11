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

	_material->Update();
}

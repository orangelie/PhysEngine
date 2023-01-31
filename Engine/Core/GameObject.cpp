#include "pch.h"
#include "GameObject.h"
#include "FrameBuffering.h"
#include "Engine.h"

void GameObject::Update()
{
	GEngine->GetFrameBuffering()->GetCurrentCbvBuffer()[(uint32)CONSTANT_BUFFER_INDEX::Transform]->PushGraphicsData(&_tranformPass, sizeof(TransformPass));
	GEngine->GetFrameBuffering()->GetCurrentGraphicsDescTable()->SetSRV(GEngine->GetTexture()->GetSrvHandle(), SRV_REGISTER::t0);
	GEngine->GetFrameBuffering()->GetCurrentGraphicsDescTable()->CommitTable();

	_material->Update();
}

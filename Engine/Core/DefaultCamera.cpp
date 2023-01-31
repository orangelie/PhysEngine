#include "pch.h"
#include "DefaultCamera.h"

void DefaultCamera::Init(uint32 width, uint32 height)
{
	Resize(width, height);

	_view = XMMatrixLookAtLH(Position, Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
	_target = Vector3(0.0f, 0.0f, 1.0f);

	_viewProj = _view * _proj;
}

void DefaultCamera::Resize(uint32 width, uint32 height)
{
	_proj = XMMatrixPerspectiveFovLH(0.25f * XM_PI, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
	_moveDirty = true;
}

void DefaultCamera::Update()
{
	if (_moveDirty)
	{
		Matrix T = XMMatrixTranslation(Position.x, Position.y, Position.z);
		_viewProj = T.Invert() * _proj;
		_moveDirty = false;
	}
}

void DefaultCamera::MoveRight(float d)
{
	Position.x += d;
	_moveDirty = true;
}

void DefaultCamera::MoveUp(float d)
{
	Position.y += d;
	_moveDirty = true;
}

void DefaultCamera::MoveForward(float d)
{
	Position.z += d;
	_moveDirty = true;
}

#include "pch.h"
#include "DefaultCamera.h"
#include "Input.h"

void DefaultCamera::Init(uint32 width, uint32 height)
{
	Resize(width, height);

	_view = XMMatrixLookAtLH(Position, Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));

	_viewProj = _view * _proj;
}

void DefaultCamera::Resize(uint32 width, uint32 height)
{
	_proj = XMMatrixPerspectiveFovLH(0.25f * XM_PI, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
	_moveDirty = true;
}

void DefaultCamera::Update()
{
	const DIMOUSESTATE mousePtDelta = Input::GetInstance()->GetMouseDelta();

	RotateY(mousePtDelta.lX * GEngine->GetGameTimer()->DeltaTime() * 2.0f);
	Pitch(mousePtDelta.lY * GEngine->GetGameTimer()->DeltaTime() * 2.0f);

	if (_moveDirty)
	{
		UpdateViewMatrix();
		_viewProj = _view * _proj;
		_moveDirty = false;
	}
}

void DefaultCamera::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&Position);
	XMStoreFloat3(&Position, XMVectorMultiplyAdd(s, r, p));

	_moveDirty = true;
}

void DefaultCamera::Walk(float d)
{
	// mPosition += d*mLook
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&Position);
	XMStoreFloat3(&Position, XMVectorMultiplyAdd(s, l, p));

	_moveDirty = true;
}

void DefaultCamera::Pitch(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	_moveDirty = true;
}

void DefaultCamera::Roll(float angle)
{
	XMMATRIX L = XMMatrixRotationAxis(XMLoadFloat3(&mLook), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), L));
	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), L));

	_moveDirty = true;
}

void DefaultCamera::RotateY(float angle)
{
	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	_moveDirty = true;
}

void DefaultCamera::UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3(&mRight);
	XMVECTOR U = XMLoadFloat3(&mUp);
	XMVECTOR L = XMLoadFloat3(&mLook);
	XMVECTOR P = XMLoadFloat3(&Position);

	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	R = XMVector3Cross(U, L);

	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
	XMStoreFloat3(&mLook, L);

	_view(0, 0) = mRight.x;
	_view(1, 0) = mRight.y;
	_view(2, 0) = mRight.z;
	_view(3, 0) = x;

	_view(0, 1) = mUp.x;
	_view(1, 1) = mUp.y;
	_view(2, 1) = mUp.z;
	_view(3, 1) = y;

	_view(0, 2) = mLook.x;
	_view(1, 2) = mLook.y;
	_view(2, 2) = mLook.z;
	_view(3, 2) = z;

	_view(0, 3) = 0.0f;
	_view(1, 3) = 0.0f;
	_view(2, 3) = 0.0f;
	_view(3, 3) = 1.0f;
}

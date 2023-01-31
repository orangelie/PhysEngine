#pragma once

#include "Engine.h"
#include "Camera.h"

class DefaultCamera : public Camera
{
public:
	DefaultCamera() {}
	virtual ~DefaultCamera() {}

	virtual Matrix ViewProj() override { return _viewProj; }

	virtual void Init(uint32 width, uint32 height) override;
	virtual void Resize(uint32 width, uint32 height) override;
	virtual void Update() override;

	virtual void Strafe(float d) override;
	virtual void Walk(float d) override;

	void Pitch(float angle);
	void Roll(float angle);
	void RotateY(float angle);

private:
	void UpdateViewMatrix();

private:
	Matrix _view = {};
	Matrix _proj = {};
	Matrix _viewProj = {};

	bool _moveDirty = true;

	Vector3 mRight = { 1.0f, 0.0f, 0.0f };
	Vector3 mUp = { 0.0f, 1.0f, 0.0f };
	Vector3 mLook = { 0.0f, 0.0f, 1.0f };

};

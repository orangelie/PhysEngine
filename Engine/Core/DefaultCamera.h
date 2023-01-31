#pragma once

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

	virtual void MoveRight(float d) override;
	virtual void MoveUp(float d) override;
	virtual void MoveForward(float d) override;

private:
	Matrix _view = {};
	Matrix _proj = {};
	Matrix _viewProj = {};

	Vector3 _target = {};

	bool _moveDirty = true;

};

#pragma once

class Camera
{
public:
	Camera() {}
	virtual ~Camera() {}

	virtual Matrix ViewProj() = 0;

	virtual void Init(uint32 width, uint32 height) = 0;
	virtual void Resize(uint32 width, uint32 height) = 0;
	virtual void Update() = 0;

	virtual void MoveRight(float d) = 0;
	virtual void MoveUp(float d) = 0;
	virtual void MoveForward(float d) = 0;

protected:
	Vector3 Position = { 0.0f, 0.0f, -10.0f };

};


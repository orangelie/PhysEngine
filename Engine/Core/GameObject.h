#pragma once

#include "Material.h"

class GameObject
{
public:
	TransformPass GetTransformPass() const { return _tranformPass; }

	void SetTransformPass(TransformPass pass) { _tranformPass = pass; }
	void SetMaterial(shared_ptr<Material> material) { _material = material; }

	void Update(Matrix View, Matrix Proj, Vector3 EyePos);

private:
	shared_ptr<Material> _material = nullptr;

	TransformPass _tranformPass = {};

};


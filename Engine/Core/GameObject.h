#pragma once

#include "Material.h"

class GameObject
{
public:
	void SetTransformPass(TransformPass pass) { _tranformPass = pass; }
	void SetMaterial(shared_ptr<Material> material) { _material = material; }

	void Update();

private:
	shared_ptr<Material> _material = nullptr;

	TransformPass _tranformPass = {};

};


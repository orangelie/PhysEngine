#pragma once

#include "Material.h"

class StaticMaterial
{
public:
	void Init();

	shared_ptr<Material> GetMaterialFromString(string name) { return _materials[name]; }

private:
	unordered_map<string, shared_ptr<Material>> _materials = {};

};


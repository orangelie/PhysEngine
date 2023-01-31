#include "pch.h"
#include "Material.h"

void Material::Update()
{
	_shader->Update();
	_mesh->Render();
}

#include "pch.h"
#include "StaticMaterial.h"

void StaticMaterial::Init()
{
#pragma region DEFAULT
	shared_ptr<Material> material = make_shared<Material>();
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init();
		StaticMesh staticMesh = {};
		staticMesh.InitAsBox(0.5f, 0.5f, 0.5f, 8);
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->Init(&staticMesh);

		material->SetShader(std::move(shader));
		material->SetMesh(std::move(mesh));
	}

	_materials["Default"] = material;
#pragma endregion
}

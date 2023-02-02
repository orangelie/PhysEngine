#include "pch.h"
#include "StaticMaterial.h"

void StaticMaterial::Init()
{
#pragma region SPHERE_DEFAULT
	shared_ptr<Material> sphere_material = make_shared<Material>();
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init();

		StaticMesh staticMesh = {};
		staticMesh.InitAsSphere(0.5f, 24);
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->Init(&staticMesh);

		sphere_material->SetShader(std::move(shader));
		sphere_material->SetMesh(std::move(mesh));
	}

	_materials["GeoSphereDefault"] = sphere_material;
#pragma endregion

#pragma region BOX_DEFAULT
	shared_ptr<Material> box_material = make_shared<Material>();
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init();

		StaticMesh staticMesh = {};
		staticMesh.InitAsBox(0.5f, 0.5f, 0.5f, 16);
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->Init(&staticMesh);

		box_material->SetShader(std::move(shader));
		box_material->SetMesh(std::move(mesh));
	}

	_materials["BoxDefault"] = box_material;
#pragma endregion
}

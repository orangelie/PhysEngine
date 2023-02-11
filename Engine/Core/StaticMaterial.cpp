#include "pch.h"
#include "StaticMaterial.h"
#include "Engine.h"

void StaticMaterial::Init()
{
#pragma region SPHERE_DEFAULT
	shared_ptr<Material> sphere_material = make_shared<Material>();
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(Resources(L"Shaders/default.hlsl").c_str(), Resources(L"Shaders/default.hlsl").c_str());

		StaticMesh staticMesh = {};
		staticMesh.InitAsSphere(0.5f, 24);
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->Init(&staticMesh);

		sphere_material->SetShader(std::move(shader));
		sphere_material->SetMesh(std::move(mesh));
		
		sphere_material->SetTexture(SRV_REGISTER::t0, GEngine->GetTextureDiffuse()->GetSrvHandle());
		sphere_material->SetTexture(SRV_REGISTER::t1, GEngine->GetTextureNormal()->GetSrvHandle());
		sphere_material->SetTexture(SRV_REGISTER::t2, GEngine->GetTextureRough()->GetSrvHandle());
	}

	_materials["GeoSphereDefault"] = sphere_material;
#pragma endregion

#pragma region BOX_DEFAULT
	shared_ptr<Material> box_material = make_shared<Material>();
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(Resources(L"Shaders/font.hlsl").c_str(), Resources(L"Shaders/font.hlsl").c_str());

		StaticMesh staticMesh = {};
		staticMesh.InitAsBox(0.5f, 0.5f, 0.5f, 16);
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->Init(&staticMesh);

		box_material->SetShader(std::move(shader));
		box_material->SetMesh(std::move(mesh));

		box_material->SetTexture(SRV_REGISTER::t0, GEngine->GetFontSrvHandle());
	}

	_materials["BoxDefault"] = box_material;
#pragma endregion
}

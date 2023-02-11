#pragma once

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

class Material
{
public:
	Material() {}
	virtual ~Material() {}

	void SetShader(shared_ptr<Shader> shader) { _shader = shader; }
	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	// 5 = CBV_REGISTER_COUNT
	void SetTexture(SRV_REGISTER reg, CD3DX12_CPU_DESCRIPTOR_HANDLE handle) { _srvHandles[static_cast<uint32>(reg) - 5] = handle; }

	void Update();

private:
	shared_ptr<Shader> _shader;
	shared_ptr<Mesh> _mesh;
	CD3DX12_CPU_DESCRIPTOR_HANDLE _srvHandles[SRV_REGISTER_COUNT] = {};

};


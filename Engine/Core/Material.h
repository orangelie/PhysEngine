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
	void SetTexture(shared_ptr<Texture> texture) { _texture = texture; }

	void Update();

private:
	shared_ptr<Shader> _shader;
	shared_ptr<Mesh> _mesh;
	shared_ptr<Texture> _texture;

};


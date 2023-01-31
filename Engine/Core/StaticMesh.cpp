#include "pch.h"
#include "StaticMesh.h"

void StaticMesh::InitAsBox(float x_half_size, float y_half_size, float z_half_size, uint32 numDivision)
{
	MeshGenerator::MeshData meshData = _meshGen.CreateBox(x_half_size, y_half_size, z_half_size, numDivision);

	_vertices = meshData.Vertices;
	_indices = meshData.Indices32;

	_meshBox.x_half = x_half_size;
	_meshBox.y_half = y_half_size;
	_meshBox.z_half = z_half_size;
}

void StaticMesh::InitAsSphere(float radius, uint32 numDivision)
{
	MeshGenerator::MeshData meshData = _meshGen.CreateGeosphere(radius, numDivision);

	_vertices = meshData.Vertices;
	_indices = meshData.Indices32;

	_meshSphere.radius = radius;
}

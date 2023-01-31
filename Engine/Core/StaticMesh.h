#pragma once

#include "MeshGenerator.h"

struct StaticMeshBox
{
	union
	{
		float x_half;
		float y_half;
		float z_half;

		float data[3];
	};
};

struct StaticMeshSphere
{
	union
	{
		float radius;

		float data[1];
	};
};

class StaticMesh
{
public:
	StaticMesh() {}
	virtual ~StaticMesh() {}

	void InitAsBox(float x_half_size, float y_half_size, float z_half_size, uint32 numDivision = 8);
	StaticMeshBox GetMeshBox() const { return _meshBox; }

	void InitAsSphere(float radius, uint32 numDivision = 16);
	StaticMeshSphere GetMeshGeoSphere() const { return _meshSphere; }


	vector<Vertex> Vertices() const { return _vertices; }
	vector<uint32> Indices() const { return _indices; }

private:
	StaticMeshBox _meshBox = {};
	StaticMeshSphere _meshSphere = {};

	MeshGenerator _meshGen = {};

	vector<Vertex> _vertices = {};
	vector<uint32> _indices = {};

};


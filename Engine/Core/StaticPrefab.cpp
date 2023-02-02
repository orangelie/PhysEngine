#include "pch.h"
#include "StaticPrefab.h"
#include "GameObject.h"
#include "Engine.h"
#include "FrameBuffering.h"
#include "StaticMaterial.h"

StaticPrefab StaticPrefab::_prefab = {};

void StaticPrefab::Init()
{
	StaticMaterial mat = {};
	mat.Init();

	shared_ptr<GameObject> boxObject1 = make_shared<GameObject>();
	{
		TransformPass pass = {};
		pass.World = XMMatrixTranslation(0.0f, -0.5f, -3.0f);

		boxObject1->SetMaterial(mat.GetMaterialFromString("BoxDefault"));
		boxObject1->SetTransformPass(pass);
	}

	_prefabs["TestCube"] = boxObject1;

	shared_ptr<GameObject> sphereObject = make_shared<GameObject>();
	{
		TransformPass pass = {};
		pass.World = XMMatrixTranslation(0.0f, 0.5f, -3.0f);

		sphereObject->SetMaterial(mat.GetMaterialFromString("GeoSphereDefault"));
		sphereObject->SetTransformPass(pass);
	}

	_prefabs["TestSphere"] = sphereObject;

	shared_ptr<GameObject> boxObject = make_shared<GameObject>();
	{
		TransformPass pass = {};
		pass.World = XMMatrixTranslation(1.0f, 0.0f, -3.0f);

		boxObject->SetMaterial(mat.GetMaterialFromString("BoxDefault"));
		boxObject->SetTransformPass(pass);
	}

	_prefabs["TestCube_tsln"] = boxObject;
}

#include "pch.h"
#include "StaticPrefab.h"
#include "GameObject.h"
#include "Engine.h"
#include "FrameBuffering.h"
#include "StaticMaterial.h"

StaticPrefab StaticPrefab::_prefab = {};

void StaticPrefab::Init()
{
	shared_ptr<GameObject> gameObject = make_shared<GameObject>();
	{
		const float _aspectRatio = (float)GEngine->GetFrameBuffering()->GetClientWidth() / (float)GEngine->GetFrameBuffering()->GetClientHeight();
		const float scaleFactor = 10.0f;
		TransformPass pass = {};
		pass.offset =
			(XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor) * XMMatrixRotationY(45.0f) * XMMatrixTranslation(0.0f, 1.0f, 1.0f)) *
			XMMatrixLookAtLH(Vector3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f)) *
			XMMatrixPerspectiveFovLH(XM_PI * 0.25f, _aspectRatio, 0.1f, 1000.0f);

		StaticMaterial mat = {};
		mat.Init();
		gameObject->SetMaterial(mat.GetMaterialFromString("Default"));
		gameObject->SetTransformPass(pass);
	}

	_prefabs["TestCube"] = std::move(gameObject);
}

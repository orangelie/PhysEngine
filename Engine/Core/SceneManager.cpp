#include "pch.h"
#include "SceneManager.h"
#include "StaticMaterial.h"
#include "Engine.h"
#include "FrameBuffering.h"
#include "StaticPrefab.h"
#include "DefaultCamera.h"

SceneManager SceneManager::_sceneManager = {};

void SceneManager::Init()
{
	CreatePrefab();
	CreateTestScene();
}

void SceneManager::CreateTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();
	{
		scene->AddGameObject("TestSphere", StaticPrefab::GetInstance()->GetPrefab("TestSphere"));
		scene->AddGameObject("TestCube_tsln", StaticPrefab::GetInstance()->GetPrefab("TestCube_tsln"));
		scene->AddGameObject("TestCube", StaticPrefab::GetInstance()->GetPrefab("TestCube"));
		
		shared_ptr<DefaultCamera> camera = make_shared<DefaultCamera>();
		{
			camera->Init(GEngine->GetFrameBuffering()->GetClientWidth(), GEngine->GetFrameBuffering()->GetClientHeight());
		}
		scene->SetCamera(camera);

		shared_ptr<Lighting> light = make_shared<Lighting>();
		{
			Light tmpLight1 = {};
			tmpLight1.Direction = Vector3(0.57735f, -0.57735f, 0.57735f);
			tmpLight1.Strength = Vector3(0.8f, 0.8f, 0.8f);

			Light tmpLight2 = {};
			tmpLight2.Direction = Vector3(-0.57735f, -0.57735f, 0.57735f);
			tmpLight2.Strength = Vector3(0.4f, 0.4f, 0.4f);

			Light tmpLight3 = {};
			tmpLight3.Direction = Vector3(0.0f, -0.707f, -0.707f);
			tmpLight3.Strength = Vector3(0.2f, 0.2f, 0.2f);

			LightPass tmpLightPass = {};
			tmpLightPass.DiffuseAlbedo = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			tmpLightPass.AmbientLight = Vector4(0.25f, 0.25f, 0.35f, 1.0f);
			tmpLightPass.Roughness = 0.2f;
			tmpLightPass.R0 = Vector3(0.05f, 0.05f, 0.05f);
			tmpLightPass.Lights[0] = tmpLight1;
			tmpLightPass.Lights[1] = tmpLight2;
			tmpLightPass.Lights[2] = tmpLight3;
			tmpLightPass.DirectionalLightCount = 3;
			tmpLightPass.PointLightCount = 0;
			tmpLightPass.SpotLightCount = 0;

			light->Init(tmpLightPass);
		}
		scene->SetLight(light);
	}

	_scenes["TestScene"] = scene;
	_activeScene = _scenes["TestScene"].get();
}

void SceneManager::CreatePrefab()
{
	StaticPrefab::GetInstance()->Init();
}

void SceneManager::Update()
{
	_activeScene->Update();
}

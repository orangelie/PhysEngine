#include "pch.h"
#include "SceneManager.h"
#include "StaticMaterial.h"
#include "Engine.h"
#include "FrameBuffering.h"
#include "StaticPrefab.h"

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
		scene->AddGameObject("TestCube", StaticPrefab::GetInstance()->GetPrefab("TestCube"));
	}

	_scenes["TestScene"] = std::move(scene);
}

void SceneManager::CreatePrefab()
{
	StaticPrefab::GetInstance()->Init();
}

void SceneManager::Update(string name)
{
	_scenes[name]->Update();
}

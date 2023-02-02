#include "pch.h"
#include "Scene.h"
#include "Engine.h"
#include "Input.h"

void Scene::Update()
{
	const float scale = 5.0f * GEngine->GetGameTimer()->DeltaTime();
	if (Input::GetInstance()->GetKeyState(DIK_W))
		_camera->Walk(scale);
	else if (Input::GetInstance()->GetKeyState(DIK_S))
		_camera->Walk(-scale);
	if (Input::GetInstance()->GetKeyState(DIK_D))
		_camera->Strafe(scale);
	else if (Input::GetInstance()->GetKeyState(DIK_A))
		_camera->Strafe(-scale);

	_camera->Update();

	for (uint32 i = 0; i < _gameObjects.size(); ++i)
	{
		_light->Update();
		_gameObjects[i].second->Update(_camera->View(), _camera->Proj(), _camera->GetEyePos());
	}
}

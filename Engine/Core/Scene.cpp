#include "pch.h"
#include "Scene.h"
#include "Engine.h"

void Scene::Update()
{
	const float scale = 5.0f * GEngine->GetGameTimer()->DeltaTime();
	if (GetAsyncKeyState('W'))
		_camera->MoveForward(scale);
	else if (GetAsyncKeyState('S'))
		_camera->MoveForward(-scale);
	if (GetAsyncKeyState('D'))
		_camera->MoveRight(scale);
	else if (GetAsyncKeyState('A'))
		_camera->MoveRight(-scale);

	_camera->Update();

	for (uint32 i = 0; i < _gameObjects.size(); ++i)
		_gameObjects[i].second->Update(_camera->ViewProj());
}

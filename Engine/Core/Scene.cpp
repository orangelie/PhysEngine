#include "pch.h"
#include "Scene.h"

void Scene::Update()
{
	for (uint32 i = 0; i < _gameObjects.size(); ++i)
		_gameObjects[i].second->Update();
}

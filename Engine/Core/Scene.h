#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Lighting.h"

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	void AddGameObject(string name, shared_ptr<GameObject> gameObject) { _gameObjects.push_back(make_pair(name, gameObject)); }

	void Update();

	void SetLight(shared_ptr<Lighting> light) { _light = light; }
	void SetCamera(shared_ptr<Camera> camera) { _camera = camera; }
	void ResizeCamera(uint32 width, uint32 height) { _camera->Resize(width, height); }

private:
	shared_ptr<Lighting> _light = nullptr;
	shared_ptr<Camera> _camera = nullptr;

	vector<pair<string, shared_ptr<GameObject>>> _gameObjects;

};

#pragma once

#include "GameObject.h"
#include "Camera.h"

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	void Update();
	void SetCamera(shared_ptr<Camera> camera) { _camera = camera; }
	void ResizeCamera(uint32 width, uint32 height) { _camera->Resize(width, height); }
	void AddGameObject(string name, shared_ptr<GameObject> gameObject) { _gameObjects.push_back(make_pair(name, gameObject)); }

private:
	vector<pair<string, shared_ptr<GameObject>>> _gameObjects;

	Vector3 _position = {};
	shared_ptr<Camera> _camera = nullptr;

};

#pragma once

#include "GameObject.h"

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	void Update();
	void AddGameObject(string name, shared_ptr<GameObject> gameObject) { _gameObjects.push_back(make_pair(name, gameObject)); }

private:
	vector<pair<string, shared_ptr<GameObject>>> _gameObjects;

};

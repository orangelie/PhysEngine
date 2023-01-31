#pragma once

#include "Scene.h"

class SceneManager
{
private:
	SceneManager() {}
	virtual ~SceneManager() {}

public:
	static SceneManager* GetInstance() { return &_sceneManager; }

public:
	void Init();
	void Update(string name);

private:
	void CreateTestScene();
	void CreatePrefab();

private:
	static SceneManager _sceneManager;

	unordered_map<string, shared_ptr<Scene>> _scenes = {};

};


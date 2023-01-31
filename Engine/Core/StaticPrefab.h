#pragma once

#include "GameObject.h"

class StaticPrefab
{
private:
	StaticPrefab() {}
	virtual ~StaticPrefab() {}

public:
	static StaticPrefab* GetInstance() { return &_prefab; }
	shared_ptr<GameObject> GetPrefab(const string& name) { return _prefabs[name]; }

public:
	void Init();

private:
	static StaticPrefab _prefab;

	unordered_map<string, shared_ptr<GameObject>> _prefabs = {};

};


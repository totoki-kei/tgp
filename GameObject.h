#pragma once

class GameObject;

#include "Game1.h"

#include "Framework/Task.h"

class GameObject : public Game1::task_list::Key {
public:
	float vx, vy, vz;
	float x, y, z;
	float vp, vt;
	float p, t;
	int reflectCount;

	int mat;
	float blend;
	float line;
	float alpha;

	bool enabled;
	std::list<GameObject>::iterator thisiter;
	GameObject();
	GameObject(bool);

	TaskResult Update();
	TaskResult Draw();

	virtual ~GameObject();
};


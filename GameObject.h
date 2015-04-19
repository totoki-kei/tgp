#pragma once

class GameObject;

#include "Game1.h"

#include "Framework/Task.h"

class GameObject {
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
	GameObject();
	GameObject(bool);

	bool Update();
	bool Draw();

	bool FlushModel();

	virtual ~GameObject();
};


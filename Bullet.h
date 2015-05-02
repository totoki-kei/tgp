#pragma once

class Bullet;

#include "GameImpl.h"
#include "GameObject.h"
#include "Misc.h"
#include <list>

class Bullet : public GameObject {
	int count;
	int materialId;
	XMFLOAT4X4 rotMatrix;

	static Models::Model* model;
	static bool staticInitialized;

public:

	static std::list<Bullet> List, Pool;
	static void StaticInit();

	Bullet();
	~Bullet();

	static Bullet& Shoot(Surface suf, int matid, XMFLOAT3 pos, XMFLOAT3 vel);

	bool enabled;


	void Update();
	void Draw();
	void Vanish();

	void UpdateRotMatrix();

	static int activeCount;
	static void UpdateAll();
	static void DrawAll();
	static void SweepToPool();
	static int GetCount();

};


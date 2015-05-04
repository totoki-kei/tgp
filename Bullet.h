#pragma once

class Bullet;

#include "GameImpl.h"
#include "GameObject.h"
#include "Misc.h"

#include "Framework/Coroutine.h"

#include <list>
#include <functional>

class Bullet : public GameObject {
public:
	typedef Coroutine<Bullet* const> PatternCoroutine;
	typedef std::function<void(PatternCoroutine::Yielder&)> Pattern;
private:
	int count;
	int materialId;
	XMFLOAT4X4 rotMatrix;

	static Models::Model* model;
	static bool staticInitialized;
	static int activeCount;

	PatternCoroutine coro;

public:
	bool lastRotated;
	float scoreRatio;

	// ÉpÉ^Å[Éì
	static void NullPattern(PatternCoroutine::Yielder& yield);
	static void AssultPattern(Bullet::PatternCoroutine::Yielder& yield);

	static std::list<Bullet> List, Pool;
	static void StaticInit();

	Bullet();
	~Bullet();

	static Bullet& Shoot(Surface suf, int matid, XMFLOAT3 pos, XMFLOAT3 vel, Pattern&& pattern = Pattern{ NullPattern });

	bool enabled;


	void Update();
	void Draw();
	void Vanish(bool noParticle = false);
	void Defeat();

	void UpdateRotMatrix();

	float GetScoreRatio();

	static void UpdateAll();
	static void DrawAll();
	static void SweepToPool();
	static int GetCount();
	static void Clear();
	static void Terminate();

};


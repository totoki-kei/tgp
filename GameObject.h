#pragma once

class GameObject;

#include "Misc.h"

class GameObject {
public:
	XMFLOAT3 pos;
	XMFLOAT3 vel;
	Surface suf;

	GameObject() = default;
	GameObject(const GameObject&) = default;
	GameObject(const XMFLOAT3 p, const XMFLOAT3 v);
	virtual ~GameObject();

	int MoveOnSurface(XMVECTOR* outQuaternion = nullptr, int maxTurn = 0, float* refDelta = nullptr);

	Surface GetSurface() const;

};


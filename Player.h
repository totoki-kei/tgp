#pragma once

class Player;

#include "GameImpl.h"
#include "GameObject.h"
#include "Session.h"

class Player : public GameObject {
	Models::Model* model;
	Session* session;

public:
	Player();
	~Player();

	XMFLOAT3 upperDirection;
	bool enabled;

	void Init(Session* session);

	void Update();
	void Draw();
};


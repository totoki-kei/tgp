#pragma once

class Player;

#include "GameImpl.h"
#include "GameObject.h"
#include "Session.h"

namespace PlayerConsts {
	const float Speed = 1 / 64.0f;
	const float PlayerSize = 0.025f;
	const float ItemCrushArea = 0.5f;
}


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

	void CheckEnemyCollide();
	void CheckItemCollide();

};


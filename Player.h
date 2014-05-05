#pragma once

#include "Framework/D3DCore.h"


class Player
{

public:
	Player();

	void Initialize(D3DCore * core);
	~Player();

	void Reload(bool selectFile = false);

};


#pragma once

#include "Framework/Graphics/D3DCore.h"


class Player
{

public:
	Player();

	void Initialize(D3DCore * core);
	~Player();

	void Reload(bool selectFile = false);

};


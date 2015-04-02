#pragma once

class Player;

#include "Framework/Graphics/D3DCore.h"
#include "Game1.h"

class Player
{
	Game1::pool_type::Item 
		task,
		modelDrawTask,
		joypadTask;
	int draw(Game1::task_type& task, Game1::task_param, void*);
	int draw_r(Game1::task_type& task, Game1::task_param, void*);

public:
	Player();

	void Initialize(D3DCore * core);
	~Player();

	void Reload(bool selectFile = false);

};


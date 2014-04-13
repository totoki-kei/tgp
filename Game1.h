#pragma once

#include "Game.h"
#include "GameWindow.h"
#include "D3DCore.h"

class Game1 : public Game
{
	GameWindow window;
	D3DCore* core;

public:
	Game1();
	~Game1();

	int Initialize();
	void Update();
	void Draw();

private:
	void InitializeD3DCore();


};


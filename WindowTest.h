#pragma once

#include "Game.h"

#include "GameWindow.h"
#include "D3DCore.h"

class WindowTest : public Game
{
	GameWindow wnd;
	D3DCore* d3d10;

public:
	WindowTest(void);
	~WindowTest(void);

	int Initialize();
	void Update();
	void Draw();


};


#pragma once

#include "Game.h"

#include "GameWindow.h"
#include "D3DCore.h"

class WindowTest : public Game
{
	GameWindow wnd;
	D3DCore* core;

public:
	WindowTest(void);
	~WindowTest(void);

	int Initialize();
	void Update();
	void Draw();

private:
	void draw1();
	void draw2();
	void draw3();
		
};


#pragma once

#include "Framework/Game.h"
#include "Framework/GameWindow.h"
#include "Framework/Graphics/D3DCore.h"

#include "Framework/Graphics/D3DShader.h"
#include "Framework/Graphics/D3DBuffer.h"

#include "Framework/Task.h"
#include "Framework/Pool.h"

#include "Framework/Graphics/Model.h"

class Game1;



class Game1 : public Game
{
public:
	typedef Task<int, void*, void*> task_type;
	typedef Pool<task_type, 4096> pool_type;
private:
	GameWindow window;
	D3DCore* core;

	int windowWidth;
	int windowHeight;

public:
	pool_type taskPool;
	task_type::container_type updateTasks;
	task_type::container_type drawTasks;
	
	static Game1* GetInstance();

public:
	Game1();
	~Game1();

	int Initialize();
	void Update();
	void Draw();

	inline int GetWindowWidth(){ return windowWidth; }
	inline int GetWindowHeight(){ return windowHeight; }
	inline HWND GetWindowHandle(){ return window.GetWindowHandle(); }

private:
	void InitializeD3DCore();


};


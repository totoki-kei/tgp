#pragma once

#include "Framework/Game.h"
#include "Framework/GameWindow.h"
#include "Framework/Graphics/D3DCore.h"

#include "Framework/Graphics/D3DShader.h"

#include "Framework/Task.h"
#include "Framework/Pool.h"

#include "Framework/Graphics/Model.h"

#include <boost/any.hpp>

class Game1;



class Game1 : public Game
{
public:
	typedef int task_ret;
	typedef boost::any task_param;
	typedef void* task_runtime_param;
	typedef TaskList<float> task_list;

private:
	GameWindow window;
	D3DCore* core;

	int windowWidth;
	int windowHeight;

public:
	
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

	float GetRand();
	float GetRand(float, float);

private:
	void InitializeD3DCore();


};


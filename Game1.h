#pragma once

#include "Game.h"
#include "GameWindow.h"
#include "D3DCore.h"

#include "D3DShader.h"
#include "D3DBuffer.h"

#include "Task.h"
#include "Pool.h"

class Game1;

struct CB_Scene {
	XMMATRIX View;
	XMMATRIX Projection;
};

struct CB_Object {
	XMMATRIX World;
};


class Game1 : public Game
{
	typedef Task<int, void*, void*> task_type;

	GameWindow window;
	D3DCore* core;

	Shaders::VertexShader *modelVS;
	Shaders::PixelShader  *modelPS;
	Shaders::PixelShader  *modelPS2;

	D3DConstantBuffer<CB_Scene>* cb_scene;
	D3DConstantBuffer<CB_Object>* cb_obj;


	Pool<task_type, 4096> taskPool;
	task_type::container_type updateTasks;
	task_type::container_type drawTasks;
	

public:
	Game1();
	~Game1();

	int Initialize();
	void Update();
	void Draw();

private:
	void InitializeD3DCore();


};


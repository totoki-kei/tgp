#include "Game1.h"


Game* GenerateGame() {
	return new Game1();
}

Game1::Game1() : window(800, 600)
{
	
}

Game1::~Game1()
{
}

int Game1::Initialize(){ 
	window.Initialize();

	InitializeD3DCore();

	return 0; 
}
void Game1::Update(){
	auto ret = window.ProcessMessage();
	if (ret != 0) exitLoop = true;
}
void Game1::Draw(){
	core->ClearRenderTarget(DirectX::XMFLOAT4(1, 0, 0, 0));
	core->Update();
}



void Game1::InitializeD3DCore(){

	core = new D3DCore(&window);

	core->Initialize(false);
}
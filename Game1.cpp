#include "Game1.h"

#include "Framework/Utility.h"

#include "Player.h"

Player *player;

Game* GenerateGame() {
	return Game1::GetInstance();

}

Game1* gm;
Game1* Game1::GetInstance(){
	if (!gm) gm = new Game1();
	return gm;
}

Game1::Game1() : windowWidth(800), windowHeight(600), window(800, 600)
{
}

Game1::~Game1()
{
	delete player;
}

int Game1::Initialize(){ 
	window.Initialize();

	InitializeD3DCore();

	SetCursor(LoadCursor(0, IDC_ARROW));

	player = new Player();
	player->Initialize(this->core);

	return 0; 
}

void Game1::Update(){
	auto ret = window.ProcessMessage();
	if (ret != 0) exitLoop = true;

	updateTasks.Invoke(nullptr);

}
void Game1::Draw(){
	//static float n = 0.0f;
	//n += 1 / 60.0f;
	//if (n >= 1) n -= 1.0f;
	core->ClearRenderTarget(DirectX::XMFLOAT4(0, 0.25, 0.5, 0));
	core->ClearDepth();

	drawTasks.Invoke(nullptr);
	drawTasks.Sweep();

	core->Update();
}



void Game1::InitializeD3DCore(){
	core = new D3DCore(&window);

	core->Initialize(true);
	core->SetVSyncWait(1);

	Models::Model::InitializeSharedResource(core);
}

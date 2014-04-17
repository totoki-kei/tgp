#include "Game1.h"

#include "Utility.h"


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

	SetCursor(LoadCursor(0, IDC_ARROW));

	return 0; 
}

void Game1::Update(){
	auto ret = window.ProcessMessage();
	if (ret != 0) exitLoop = true;

	updateTasks.Invoke(nullptr);

}
void Game1::Draw(){
	static float n = 0.0f;
	n += 1 / 60.0f;
	if (n >= 1) n -= 1.0f;
	core->ClearRenderTarget(DirectX::XMFLOAT4(n, 0, 0, 0));

	drawTasks.Invoke(nullptr);

	core->Update();
}



void Game1::InitializeD3DCore(){
	core = new D3DCore(&window);

	core->Initialize(false);
	core->SetVSyncWait(1);

	// initialize shaders
	modelVS = Shaders::Load<Shaders::VertexShader>(core, _T("VS_Transform.cso"));
	core->AddResource(PtrToRes(modelVS));

	modelPS = Shaders::Load<Shaders::PixelShader>(core, _T("PS_NormalColor.cso"));
	core->AddResource(PtrToRes(modelPS));

	modelPS2 = Shaders::Load<Shaders::PixelShader>(core, _T("PS_EmitColor.cso"));
	core->AddResource(PtrToRes(modelPS2));

	cb_scene = new D3DConstantBuffer<CB_Scene>(core);
	core->AddResource(PtrToRes(cb_scene));

	cb_obj = new D3DConstantBuffer<CB_Object>(core);
	core->AddResource(PtrToRes(cb_obj));

}

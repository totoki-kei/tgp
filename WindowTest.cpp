#include "WindowTest.h"

#include "Debug.h"

Game* GenerateGame() {
	return new WindowTest();
}

#define TEST_ID 1

#if TEST_ID == 0
WindowTest::WindowTest(void) : wnd(640, 480)
{
	wnd.SetWindowTitle(TEXT("ほげほげほげ"));
	GameWindow::SetMessageHandler(
		WM_KEYDOWN,
		[this](UINT msg, WPARAM wp, LPARAM lp) {
			DBG_OUT("msg = %08X, WPARAM = %08X, LPARAM = %08X\n", msg, wp, lp);
			
			if(wp == VK_ESCAPE) {
				this->wnd.SetWindowTitle("Escaped.");
				GameWindow::ResetMessageHandler(WM_KEYDOWN);
			}

			return 0;
		} 
	);

	GameWindow::SetMessageHandler(
		WM_MOUSEMOVE,
		[this](UINT msg, WPARAM wp, LPARAM lp) {
			DBG_OUT("[%10d] msg = %08X, WPARAM = %08X, LPARAM = %08X\n", GetMessageTime(), msg, wp, lp);
			
			return 0;
		} 
	);

}


WindowTest::~WindowTest(void)
{
	wnd.Dispose();
}

int WindowTest::Initialize(void){
	wnd.Initialize();
	d3d10 = new D3DCore(&wnd);
	d3d10->Initialize();
	return 0;
}

void WindowTest::Update(void) {

	auto ret = wnd.ProcessMessage();
	if(ret != 0) exitLoop = true;

	// ここでフレームごとの処理を行う
	d3d10->Clear();
	d3d10->Update();
	
}

void WindowTest::Draw(void) {

}

#elif TEST_ID == 1

#include "D3DBuffer.h"
#include "D3DEffect.h"
#include "D3DInputLayout.h"
#include "Effect_Effect1.h"

WindowTest::WindowTest(void) : wnd(640, 480)
{
	wnd.SetWindowTitle(TEXT("ほげほげほげ"));
	GameWindow::SetMessageHandler(
		WM_KEYDOWN,
		[this](UINT msg, WPARAM wp, LPARAM lp) {
		DBG_OUT("msg = %08X, WPARAM = %08X, LPARAM = %08X\n", msg, wp, lp);

		if (wp == VK_ESCAPE) {
			this->wnd.SetWindowTitle("Escaped.");
			GameWindow::ResetMessageHandler(WM_KEYDOWN);
		}

		return 0;
	}
	);

	GameWindow::SetMessageHandler(
		WM_MOUSEMOVE,
		[this](UINT msg, WPARAM wp, LPARAM lp) {
		DBG_OUT("[%10d] msg = %08X, WPARAM = %08X, LPARAM = %08X\n", GetMessageTime(), msg, wp, lp);

		return 0;
	}
	);

}

struct Vertex {
	D3DXVECTOR4 position;
	D3DXCOLOR color;
	D3DXCOLOR emit;

	static D3D10_INPUT_ELEMENT_DESC* GetInputElementDesc(){
		static D3D10_INPUT_ELEMENT_DESC desc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "EMIT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		};
		return desc;
	}
	static int GetInputElementDescCount(){ return 3; }
};

struct CB_Scene {
	D3DXMATRIX View;
	D3DXMATRIX Projection;
};

struct CB_Object {
	D3DXMATRIX World;
};


D3DEffect* effect;
D3DEffect::Technique tech;
D3DVertexBuffer<Vertex> * vb;
D3DInputLayout* ia;

WindowTest::~WindowTest(void)
{
	delete ia;
	delete vb;
	delete effect;

	wnd.Dispose();
}

int WindowTest::Initialize(void){
	wnd.Initialize();
	d3d10 = new D3DCore(&wnd);
	d3d10->Initialize();

	// ためしにエフェクトを読んでみよう
	effect = new D3DEffect(d3d10, "Debug\\Effect1.fxo");
	tech = effect->GetTechnique(0);


	Vertex vs[4] = {
		{ {  0 ,  0 , 0, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } },
		{ {  0 , .5f, 0, 1 }, { 1, 0, 1, 1 }, { 0, 1, 0, 1 } },
		{ { .5f, 0, 0, 1 }, { 0, 1, 1, 1 }, { 1, 0, 0, 1 } },
		{ { .5f, .5f, 0, 1 }, { 1, 1, 0, 1 }, { 0, 0, 1, 1 } },
	};

	vb = new D3DVertexBuffer<Vertex>(d3d10, vs);

	ia = new D3DInputLayout(d3d10, Vertex::GetInputElementDesc(), Vertex::GetInputElementDescCount(), tech, 0);

	return 0;
}

void WindowTest::Update(void) {

	auto ret = wnd.ProcessMessage();
	if (ret != 0) exitLoop = true;

	// ここでフレームごとの処理を行う
	d3d10->Clear();

	d3d10->SetPrimitiveTopology(D3DPrimitiveTopology::TriangleStrip);
	vb->Apply();
	ia->Apply();

	// エフェクトのパラメータ
	{
		auto obj = effect->GetConstantBuffer<CB_Object>("Object");
		auto op = obj.lock();
		auto or = op->GetPointer();
		D3DXMatrixRotationZ(&or->World, this->ticks / 32.0f);
		op->Update();

		auto scene = effect->GetConstantBuffer<CB_Scene>("Scene");
		auto sp = scene.lock();
		auto sr = sp->GetPointer();
		D3DXMatrixIdentity(&sr->View);
		D3DXMatrixIdentity(&sr->Projection);
		sp->Update();
	}

	tech.SetPass(0);
	tech.ApplyPass();

	// テストコード（のちのちD3DCoreにラップする)
	auto device = d3d10->GetDevice();
	device->Draw(4, 0);

	d3d10->Update();

}

void WindowTest::Draw(void) {

}

#endif

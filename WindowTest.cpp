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
#include "D3DShader.h"
#include "D3DInputLayout.h"
#include "D3DTexture.h"
#include "D3DSampler.h"
#include "D3DStencilState.h"

WindowTest::WindowTest(void) : wnd(800, 600)
{
	wnd.SetWindowTitle(_T("ほげほげほげ"));
	GameWindow::SetMessageHandler(
		WM_KEYDOWN,
		[this](UINT msg, WPARAM wp, LPARAM lp) {
			DBG_OUT("msg = %08X, WPARAM = %08X, LPARAM = %08X\n", msg, wp, lp);
	
			if (wp == VK_ESCAPE) {
				this->wnd.SetWindowTitle(_T("Escaped."));
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
	XMFLOAT4 position;
	XMFLOAT4 color;
	XMFLOAT4 emit;

	static D3D11_INPUT_ELEMENT_DESC* GetInputElementDesc(){
		static D3D11_INPUT_ELEMENT_DESC desc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "EMIT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		return desc;
	}
	static int GetInputElementDescCount(){ return 3; }
};

struct CB_Scene {
	XMMATRIX View;
	XMMATRIX Projection;
};

struct CB_Object {
	XMMATRIX World;
};


D3DVertexBuffer<Vertex> * vb;
D3DIndexBuffer<> * ib;
D3DInputLayout* ia;

Shaders::VertexShader* vs;
Shaders::PixelShader* ps;
Shaders::PixelShader* ps2;
D3DConstantBuffer<CB_Scene>* cb_scene;
D3DConstantBuffer<CB_Object>* cb_obj;


WindowTest::~WindowTest(void)
{

	wnd.Dispose();
}

int WindowTest::Initialize(void){
	using namespace std;

	wnd.Initialize();
	d3d10 = new D3DCore(&wnd);
	d3d10->Initialize();

	vs = Shaders::Load<Shaders::VertexShader>(d3d10, _T("VS_Transform.cso"));
	wnd.AddResource(shared_ptr<Resource>(vs));

	ps = Shaders::Load<Shaders::PixelShader>(d3d10, _T("PS_NormalColor.cso"));
	wnd.AddResource(shared_ptr<Resource>(ps));

	ps2 = Shaders::Load<Shaders::PixelShader>(d3d10, _T("PS_EmitColor.cso"));
	wnd.AddResource(shared_ptr<Resource>(ps2));

	cb_scene = new D3DConstantBuffer<CB_Scene>(d3d10);
	wnd.AddResource(shared_ptr<Resource>(cb_scene));

	cb_obj = new D3DConstantBuffer<CB_Object>(d3d10);
	wnd.AddResource(shared_ptr<Resource>(cb_obj));

	Vertex vertices[4] = {
		{ {  0 ,  0 , 0, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } },
		{ {  0 , .5f, 0, 1 }, { 1, 0, 1, 1 }, { 0, 1, 0, 1 } },
		{ { .5f, 0, 0, 1 }, { 0, 1, 1, 1 }, { 1, 0, 0, 1 } },
		{ { .5f, .5f, 0, 1 }, { 1, 1, 0, 1 }, { 0, 0, 1, 1 } },
	};

	vb = new D3DVertexBuffer<Vertex>(d3d10, vertices);
	wnd.AddResource(shared_ptr<Resource>(vb));

	ib = new D3DIndexBuffer<>(d3d10, 5);
	wnd.AddResource(shared_ptr<Resource>(ib));

	unsigned short indices[] = { 0, 1, 3, 2, 0 };
	ib->Update(indices);

	ia = new D3DInputLayout(d3d10, Vertex::GetInputElementDesc(), Vertex::GetInputElementDescCount(), vs, 0);
	wnd.AddResource(shared_ptr<Resource>(ia));

	return 0;
}

void WindowTest::Update(void) {

	auto ret = wnd.ProcessMessage();
	if (ret != 0) exitLoop = true;

	// ここでフレームごとの処理を行う
	d3d10->Clear();

	vb->Apply();
	ib->Apply();
	ia->Apply();

	CB_Scene s;
	s.View = XMMatrixIdentity();
	s.Projection = XMMatrixIdentity();
	cb_scene->Update(&s);
	cb_scene->Apply(Shaders::ShaderFlag::All, 0);

	CB_Object o;
	//o.World = XMMatrixRotationZ(GetTickCount() / 256.0f);
	o.World = XMMatrixRotationZ(GetTickCount() / 1000.0f * XM_2PI);
	cb_obj->Update(&o);
	cb_obj->Apply(Shaders::ShaderFlag::All, 1);

	// テストコード（のちのちD3DCoreにラップする)
	//auto device = d3d10->GetDeviceContext();

	vs->Apply();

	ps->Apply();
	d3d10->SetPrimitiveTopology(D3DPrimitiveTopology::TriangleStrip);
	d3d10->Draw(4, 0);


	o.World = XMMatrixTranslation(0, 0, 0.5f) * XMMatrixRotationZ(GetTickCount() / 1500.0f * XM_2PI);
	cb_obj->Update(&o);

	ps2->Apply();
	d3d10->SetPrimitiveTopology(D3DPrimitiveTopology::LineStrip);
	d3d10->DrawIndexed(5, 0, 0);

	d3d10->Update();

}

void WindowTest::Draw(void) {

}

#endif

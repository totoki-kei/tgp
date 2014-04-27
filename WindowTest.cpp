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
<<<<<<< HEAD
	d3d10 = new D3DCore(&wnd);
	d3d10->Initialize();
=======
	core = new D3DCore(&wnd);
	core->Initialize();
>>>>>>> DX11
	return 0;
}

void WindowTest::Update(void) {

	auto ret = wnd.ProcessMessage();
	if(ret != 0) exitLoop = true;

	// ここでフレームごとの処理を行う
<<<<<<< HEAD
	d3d10->Clear();
	d3d10->Update();
=======
	core->Clear();
	core->Update();
>>>>>>> DX11
	
}

void WindowTest::Draw(void) {

}

#elif TEST_ID == 1

<<<<<<< HEAD
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
=======
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include "Program.h"
#include "D3DBuffer.h"
#include "D3DShader.h"
#include "D3DInputLayout.h"
#include "D3DTexture.h"
#include "D3DSampler.h"
#include "D3DStencilState.h"

#include "Model.h"

#include "resource1.h"

HICON icon;

WindowTest::WindowTest(void) : wnd(WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, icon = LoadIcon(g_MainArgs.hInstance, MAKEINTRESOURCE(IDI_ICON1)))
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
>>>>>>> DX11
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
<<<<<<< HEAD
	D3DXVECTOR4 position;
	D3DXCOLOR color;
	D3DXCOLOR emit;

	static D3D10_INPUT_ELEMENT_DESC* GetInputElementDesc(){
		static D3D10_INPUT_ELEMENT_DESC desc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "EMIT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0 },
=======
	XMFLOAT4 position;
	XMFLOAT4 color;
	XMFLOAT4 emit;

	static D3D11_INPUT_ELEMENT_DESC* GetInputElementDesc(){
		static D3D11_INPUT_ELEMENT_DESC desc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "EMIT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
>>>>>>> DX11
		};
		return desc;
	}
	static int GetInputElementDescCount(){ return 3; }
};

<<<<<<< HEAD
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
=======
using std::shared_ptr;

D3DVertexBuffer<Vertex> * vb;
D3DIndexBuffer<> * ib;
D3DInputLayout* ia;

typedef Models::SceneParameter CB_Scene;
typedef Models::ObjectParameter CB_Object;
typedef Models::SubsetParameter CB_Subset;

Shaders::VertexShader* vs;
Shaders::GeometryShader* gs;
Shaders::PixelShader* ps;
Shaders::PixelShader* ps2;
D3DConstantBuffer<CB_Scene>* cb_scene;
D3DConstantBuffer<CB_Object>* cb_obj;
D3DConstantBuffer<CB_Subset>* cb_ss;


Shaders::VertexShader* e_vs;
Shaders::GeometryShader* e_gs;
Shaders::PixelShader* e_ps;
D3DTexture2D* e_rt;
D3D11_VIEWPORT e_vp;
D3DStencilState *e_st;
D3DSampler* e_sm;

WindowTest::~WindowTest(void)
{
	timeEndPeriod(1);
>>>>>>> DX11

	wnd.Dispose();
}

int WindowTest::Initialize(void){
<<<<<<< HEAD
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
=======

	wnd.Initialize();
	core = new D3DCore(&wnd);
	core->Initialize(true);

	vs = Shaders::Load<Shaders::VertexShader>(core, _T("VS_Transform.cso"));
	wnd.AddResource(shared_ptr<Resource>(vs));

	gs = Shaders::Load<Shaders::GeometryShader>(core, _T("GS_CreateEdge.cso"));
	wnd.AddResource(shared_ptr<Resource>(gs));

	ps = Shaders::Load<Shaders::PixelShader>(core, _T("PS_NormalColor.cso"));
	wnd.AddResource(shared_ptr<Resource>(ps));

	ps2 = Shaders::Load<Shaders::PixelShader>(core, _T("PS_EmitColor.cso"));
	wnd.AddResource(shared_ptr<Resource>(ps2));

	cb_scene = new D3DConstantBuffer<CB_Scene>(core);
	wnd.AddResource(shared_ptr<Resource>(cb_scene));

	cb_obj = new D3DConstantBuffer<CB_Object>(core);
	wnd.AddResource(shared_ptr<Resource>(cb_obj));

	cb_ss = new D3DConstantBuffer<CB_Subset>(core);
	wnd.AddResource(shared_ptr<Resource>(cb_ss));

	Vertex vertices[4] = {
		{ {  0 ,  0 , 0, 1 }, {  1,   1,   1,  1 }, { 0, 0, 0, 1 } },
		{ {  0 , .5f, 0, 1 }, {  1,   0,   1,  1 }, { 0, 1, 0, 1 } },
		{ { .5f,  0 , 0, 1 }, {  0,   1,   1,  1 }, { 1, 0, 0, 1 } },
		{ { .5f, .5f, 0, 1 }, {  1,   1,   0,  1 }, { 0, 0, 1, 1 } },
	};

	vb = new D3DVertexBuffer<Vertex>(core, vertices);
	wnd.AddResource(shared_ptr<Resource>(vb));

	ib = new D3DIndexBuffer<>(core, 5);
	wnd.AddResource(shared_ptr<Resource>(ib));

	D3DIndexBuffer<>::index_t indices[] = { 0, 1, 3, 2, 0 };
	ib->Update(indices);

	ia = new D3DInputLayout(core, Vertex::GetInputElementDesc(), Vertex::GetInputElementDescCount(), vs, 0);
	wnd.AddResource(shared_ptr<Resource>(ia));

	// テクスチャを使う方のシェーダ
	e_vs = Shaders::Load<Shaders::VertexShader>(core, _T("VS_ThroughID.cso"));
	e_gs = Shaders::Load<Shaders::GeometryShader>(core, _T("GS_ID2Quad.cso"));
	e_ps = Shaders::Load<Shaders::PixelShader>(core, _T("PS_TextureOnly.cso"));
	wnd.AddResource(shared_ptr<Resource>(e_vs));
	wnd.AddResource(shared_ptr<Resource>(e_gs));
	wnd.AddResource(shared_ptr<Resource>(e_ps));

	//e_rt = new D3DTexture2D(core, WINDOW_WIDTH / 8, WINDOW_HEIGHT / 8, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
	e_rt = new D3DTexture2D(
		core, 
		WINDOW_WIDTH / 8, 
		WINDOW_HEIGHT / 8,
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		D3D11_RESOURCE_MISC_GDI_COMPATIBLE);
	wnd.AddResource(shared_ptr<Resource>(e_rt));

	e_st = new D3DStencilState(core, false, false);
	wnd.AddResource(shared_ptr<Resource>(e_st));
	
	{
		D3D11_SAMPLER_DESC desc;
		D3DSampler::GetDefaultSamplerDesc(&desc);

		desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;

		e_sm = new D3DSampler(core, &desc);
	}
	wnd.AddResource(shared_ptr<Resource>(e_sm));


	core->GetDefaultViewport(&e_vp);
	e_vp.Width = WINDOW_WIDTH / 8;
	e_vp.Height = WINDOW_HEIGHT / 8;

	core->SetVSyncWait(1);


	SetCursor(LoadCursor(0, IDC_ARROW));

	auto hr = timeBeginPeriod(1);

>>>>>>> DX11

	return 0;
}

<<<<<<< HEAD
=======
void WindowTest::draw1(){
	CB_Scene s;
	CB_Object o;
	CB_Subset ss;
	s.View = XMMatrixIdentity();
	s.Projection = XMMatrixOrthographicLH(WINDOW_WIDTH / 200, WINDOW_HEIGHT / 200, 0, 10);
	cb_scene->Update(&s);
	cb_scene->Apply(Shaders::ShaderFlag::All, 0);
//	e_sm->Unapply(Shaders::ShaderFlag::All, 0);

	//o.World = XMMatrixRotationZ(ticks / 256.0f);
	//o.World = XMMatrixRotationZ(timeGetTime() / 1000.0f * XM_2PI);
	o.World = XMMatrixRotationZ((timeGetTime() % 1000) * XM_2PI / 1000.0f);
	cb_obj->Update(&o);
	cb_obj->Apply(Shaders::ShaderFlag::All, 1);

	ss.BaseColor = XMFLOAT4(0, 0, 0, 0);
	ss.EdgeGradient = XMFLOAT4(1, 1, 1, 0);
	cb_ss->Update(&ss);
	cb_ss->Apply(Shaders::ShaderFlag::All, 2);

	vs->Apply();
	gs->Apply();
	e_st->Unapply();

	ps->Apply();
	core->SetPrimitiveTopology(D3DPrimitiveTopology::TriangleStrip);
	core->Draw(4, 0);

}

void WindowTest::draw2(){

	CB_Scene s;
	CB_Object o;
	CB_Subset ss;
	s.View = XMMatrixIdentity();
	s.Projection = XMMatrixOrthographicLH(WINDOW_WIDTH / 200, WINDOW_HEIGHT / 200, 0, 10);
	cb_scene->Update(&s);
	cb_scene->Apply(Shaders::ShaderFlag::All, 0);
	//e_sm->Unapply(Shaders::ShaderFlag::All, 0);

	//o.World = XMMatrixRotationZ(ticks / 256.0f);
	o.World = XMMatrixRotationZ((timeGetTime() % 1000) * XM_2PI / 1000.0f);
	cb_obj->Update(&o);
	cb_obj->Apply(Shaders::ShaderFlag::All, 1);

	ss.BaseColor = XMFLOAT4(0, 0, 0, 0);
	ss.EdgeGradient = XMFLOAT4(0, 0, 0, 1);
	cb_ss->Update(&ss);
	cb_ss->Apply(Shaders::ShaderFlag::All, 2);


	vs->Apply();
	gs->Apply();
	//e_st->Unapply();

	ps2->Apply();
	core->SetPrimitiveTopology(D3DPrimitiveTopology::TriangleStrip);
	//core->DrawIndexed(5, 0, 0);
	core->Draw(4, 0);

}

void WindowTest::draw3(){
	e_rt->Apply(Shaders::ShaderFlag::Pixel, 0);

	e_vs->Apply();
	e_gs->Apply();
	e_ps->Apply();
	core->SetPrimitiveTopology(D3DPrimitiveTopology::PointList);

	e_st->Apply();
	e_sm->Apply(Shaders::ShaderFlag::All, 0);

	core->Draw(1, 0);
	e_rt->Unapply(Shaders::ShaderFlag::Pixel, 0);


}

>>>>>>> DX11
void WindowTest::Update(void) {

	auto ret = wnd.ProcessMessage();
	if (ret != 0) exitLoop = true;

<<<<<<< HEAD
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
=======
	//Sleep(100);
	// ここでフレームごとの処理を行う


	vb->Apply();
	ib->Apply();
	ia->Apply();

	e_rt->SetToRenderTarget();
	core->SetViewport(&e_vp);
	core->ClearDepth();
	e_rt->ClearAsRenderTarget(XMFLOAT4(0, 0, 0, 0));

	draw2();
	

	e_rt->DrawAsDc([this](HDC hdc, RECT** rect) {
		*rect = new RECT;
		(*rect)->top = 0;
		(*rect)->bottom = 75;
		(*rect)->left = 0;
		(*rect)->right = 100;

		COLORREF color = RGB(255, 0, 255);
		SetTextColor(hdc, color);

		TCHAR str[32];
		int len = _stprintf_s(str, _T("%f"), (timeGetTime() % 1000) * XM_2PI / 1000.0f);

		DrawText(hdc, str, -1, *rect, DT_CENTER);

	});

	// ===========================================
	core->SetDefaultRenderTarget();
	core->SetDefaultViewport();
	core->ClearRenderTarget(XMFLOAT4(1, 1, 0, 1));
	core->ClearDepth();

	draw3();
	draw1();

	core->Update();
>>>>>>> DX11

}

void WindowTest::Draw(void) {

}

#endif

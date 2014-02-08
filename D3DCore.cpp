#include "D3DCore.h"
#include "DirectXUtil.h"

#include "Debug.h"

D3DCore::D3DCore(GameWindow* wnd)
{
	this->wnd = wnd;
	wnd->AddResource(PtrToRes(this));
}


D3DCore::~D3DCore(void)
{
	if (!isDisposed()) Dispose();
}

bool D3DCore::Initialize() {
	HRESULT hr;

	// スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC scd = {0};
	scd.BufferCount = 1;
	
	scd.BufferDesc.Height = wnd->GetHeight();
	scd.BufferDesc.Width = wnd->GetWidth();
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = wnd->GetWindowHandle();

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Windowed = TRUE;

	D3D_FEATURE_LEVEL flevels[] = { D3D_FEATURE_LEVEL_10_1 };

	// デバイスとスワップチェインの作成
	IF_NG2(D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		nullptr, 0,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&device,
		flevels,
		&cxt), hr) {
			// 失敗


			return false;
	}

	auto dev = HndToRes(device);
	AddResource(dev);
	AddResource(HndToRes(swapChain));

	

	// バックバッファの作成
	ID3D11Texture2D *backbuffer;
	IF_NG2(swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D), 
		(void**)&backbuffer) , hr) {

			// 失敗
			return false;
	}
	IF_NG2(device->CreateRenderTargetView(backbuffer, NULL, &rtv), hr){
			// 失敗
			return false;
	}

	cxt->OMSetRenderTargets(1, &rtv, NULL);
	IF_NG2(backbuffer->Release(), hr){
			// 失敗
			return false;
	};

	dev->AddResource(HndToRes(rtv));

	// ビューポートの作成
	viewport.Width = (float)wnd->GetWidth();
	viewport.Height = (float)wnd->GetHeight();
	viewport.TopLeftX = viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	cxt->RSSetViewports(1, &viewport);

	// 画面消去をするときの色
	this->clearColor.r = 0.5f;
	this->clearColor.g = 0.5f;
	this->clearColor.b = 0.5f;
	this->clearColor.a = 1.0f;

	return true;
	
}

bool D3DCore::isDisposed() {
	return device == nullptr;
}

void D3DCore::Dispose() {
	if(isDisposed()) return;
	cxt->ClearState();
	Resource::Dispose();
	device = nullptr;
}

void D3DCore::Clear() {
	cxt->ClearRenderTargetView(rtv, clearColor);
}

void D3DCore::Update() {
	swapChain->Present(0, 0);
}

ID3D11Device* D3DCore::GetDevice() const {
	return this->device;
}

ID3D11DeviceContext* D3DCore::GetDeviceContext() const {
	return this->cxt;
}

void D3DCore::SetPrimitiveTopology(D3DPrimitiveTopology topology){
	cxt->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)topology);
}

#include "D3DCore.h"
#include "D3DLibs.h"
#include "../DirectXUtil.h"

#include "../Debug.h"

#include <dwmapi.h>

D3DCore::D3DCore(GameWindow* wnd)
{
	this->wnd = wnd;

	// ウィンドウ削除時には自分も死ぬようにする
	wnd->AddResource(PtrToRes(this));
}


D3DCore::~D3DCore(void)
{
	if (!isDisposed()) Dispose();
}

bool D3DCore::Initialize(bool fullscreen, bool debugDevice) {
	HRESULT hr;

	// スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferCount = 1;

	scd.BufferDesc.Height = wnd->GetHeight();
	scd.BufferDesc.Width = wnd->GetWidth();
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	{
		// 一番近いディスプレイモードを検索
		IDXGIFactory* dxgiFactory = 0;
		IDXGIAdapter* dxgiAdapter = 0;
		IDXGIOutput* dxgiOutput = 0;

		// ファクトリを取得
		CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

		// アダプタを取得
		dxgiFactory->EnumAdapters(0, &dxgiAdapter);

		// 
		dxgiAdapter->EnumOutputs(0, &dxgiOutput);

		DXGI_MODE_DESC closest;
		HRESULT mode_hr = dxgiOutput->FindClosestMatchingMode(&scd.BufferDesc, &closest, nullptr);
		IF_OK(mode_hr) {
			scd.BufferDesc = closest;
		}

		if (dxgiOutput) dxgiOutput->Release();
		if (dxgiAdapter) dxgiAdapter->Release();
		if (dxgiFactory) dxgiFactory->Release();
	}

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = wnd->GetWindowHandle();

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Windowed = fullscreen ? FALSE : TRUE;



	D3D_FEATURE_LEVEL flevels[] = { D3D_FEATURE_LEVEL_10_1 };
	//D3D_FEATURE_LEVEL flevels[] = { D3D_FEATURE_LEVEL_11_0 };

	/*
	 * TODO:
	 *  デバイスタイプをハードウェアにするかWARPにするか
	 *  引数で区別できるようにする
	 *  (可能であれば、自動推論もできるように)
	 */

	// デバイスとスワップチェインの作成
	IF_NG2(D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_WARP,
		// D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		debugDevice ? D3D11_CREATE_DEVICE_DEBUG : 0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&device,
		flevels,
		&cxt), hr) {
		// 失敗


		return false;
	}

	NameToResource(device, "MainDevice");
	auto dev = HndToRes(device);
	AddResource(dev);

	// デバッグ インターフェースの取得
	if (debugDevice) {
		device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3ddbg));
		dev->AddResource(std::make_shared< ResourceItem<ID3D11Debug*>>(d3ddbg, [](ID3D11Debug* d) {
			d->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			d->Release();
			d = nullptr;
		}));
	}
	else {
		d3ddbg = nullptr;
	}


	NameToResource(cxt, "MainDeviceContext");
	dev->AddResource(HndToRes(cxt));
	NameToResource(swapChain, "MainSwapchain");
	dev->AddResource(HndToRes(swapChain));



	// バックバッファの作成

	ID3D11Texture2D *backbuffer;
	IF_NG2(swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&backbuffer), hr) {

		// 失敗
		return false;
	}
	IF_NG2(device->CreateRenderTargetView(backbuffer, NULL, &rtv), hr){
		// 失敗
		return false;
	}
	NameToResource(rtv, "DefaultRenderTargetView");
	dev->AddResource(HndToRes(rtv));

	// 深度バッファの作成
	CD3D11_TEXTURE2D_DESC txd;
	{
		// data unit
		txd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		txd.Height = scd.BufferDesc.Height;
		txd.Width = scd.BufferDesc.Width;

		// sampling
		txd.SampleDesc = scd.SampleDesc;

		// array, mipmap
		txd.ArraySize = 1;
		txd.MipLevels = 1;
		txd.MiscFlags = 0;

		// access
		txd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		txd.CPUAccessFlags = 0U;
		txd.Usage = D3D11_USAGE_DEFAULT;
	}
	IF_NG2(device->CreateTexture2D(&txd, nullptr, &this->depthStencil), hr){
		return false;
	}
	NameToResource(depthStencil, "DefaultDepthStencil");
	dev->AddResource(HndToRes(depthStencil));

	IF_NG2(device->CreateDepthStencilView(this->depthStencil, nullptr, &this->dsv), hr){
		return false;
	}
	NameToResource(dsv, "DefaultDepthStencilView");
	dev->AddResource(HndToRes(dsv));

	// バックバッファと深度バッファを設定
	cxt->OMSetRenderTargets(1, &rtv, dsv);
	IF_NG2(backbuffer->Release(), hr){
		// 失敗
		return false;
	};

	// ビューポートの作成
	viewport.Width = (float)wnd->GetWidth();
	viewport.Height = (float)wnd->GetHeight();
	viewport.TopLeftX = viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	cxt->RSSetViewports(1, &viewport);

	// ブレンドステート(通常)
	// DirectX11のデフォルト値を使用する
	bsNormal = nullptr;

	// ブレンドステート(アルファ)
	D3D11_BLEND_DESC bd;
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	{
		auto tg = &bd.RenderTarget[0];
		tg->BlendEnable = true;
		
		tg->BlendOp = D3D11_BLEND_OP_ADD;
		tg->SrcBlend = D3D11_BLEND_SRC_ALPHA;
		tg->DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		tg->BlendOpAlpha = D3D11_BLEND_OP_ADD;
		tg->SrcBlendAlpha = D3D11_BLEND_ONE;
		tg->DestBlendAlpha = D3D11_BLEND_ONE;
		
		tg->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	IF_NG2(device->CreateBlendState(&bd, &this->bsAlpha), hr){
		return false;
	}
	NameToResource(bsAlpha, "AlphaBlendState");
	dev->AddResource(HndToRes(bsAlpha));

	// ブレンドステート(加算合成)
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	{
		auto tg = &bd.RenderTarget[0];
		tg->BlendEnable = true;

		tg->BlendOp = D3D11_BLEND_OP_ADD;
		tg->SrcBlend = D3D11_BLEND_SRC_ALPHA;
		tg->DestBlend = D3D11_BLEND_ONE;

		tg->BlendOpAlpha = D3D11_BLEND_OP_ADD;
		tg->SrcBlendAlpha = D3D11_BLEND_ONE;
		tg->DestBlendAlpha = D3D11_BLEND_ONE;

		tg->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	IF_NG2(device->CreateBlendState(&bd, &this->bsAdd), hr) {
		return false;
	}
	NameToResource(bsAdd, "AdditiveBlendState");
	dev->AddResource(HndToRes(bsAdd));

	// 初期ブレンドステートの設定
	// 初期値はアルファブレンド
	SetBlendMode(D3DBlendMode::Alpha);


	// 画面消去をするときの色
	this->clearColor[0] = 0.0f;
	this->clearColor[1] = 0.0f;
	this->clearColor[2] = 0.0f;
	this->clearColor[3] = 1.0f;

	// 初期ステートの設定
	this->SetDefaultRenderTarget();
	this->SetDefaultViewport();

	// VSync待ち(デフォルトは待たない)
	this->vsyncWait = 0;

	return true;

}

bool D3DCore::isDisposed() {
	return device == nullptr;
}

void D3DCore::Dispose() {
	if (isDisposed()) return;
	cxt->ClearState();
	device = nullptr;
	Resource::Dispose();
}

void D3DCore::Clear() {
	cxt->ClearRenderTargetView(rtv, clearColor);
	cxt->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}


void D3DCore::ClearRenderTarget(){
	cxt->ClearRenderTargetView(rtv, clearColor);
}

void D3DCore::SetClearColor(XMFLOAT4 color){
	clearColor[0] = color.x;
	clearColor[1] = color.y;
	clearColor[2] = color.z;
	clearColor[3] = color.w;
}

void D3DCore::ClearRenderTarget(XMFLOAT4 color){
	FLOAT f[] = { color.x, color.y, color.z, color.w };
	cxt->ClearRenderTargetView(rtv, f);
}

void D3DCore::ClearDepth(){
	cxt->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}



void D3DCore::Update() {
	swapChain->Present(this->vsyncWait, 0);
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

void D3DCore::Draw(int vertexCount, int offset){
	cxt->Draw(vertexCount, offset);
}

void D3DCore::DrawIndexed(int indexCount, int vertexOffset, int indexOffset) {
	cxt->DrawIndexed(indexCount, indexOffset, vertexOffset);
}

void D3DCore::DrawIndexed(int indexCount, int vertexOffset, int indexOffset, int instanceCount) {
	cxt->DrawIndexedInstanced(indexCount, instanceCount, indexOffset, vertexOffset, 0);
}

void D3DCore::SetVSyncWait(int wait){ this->vsyncWait = wait; }
int D3DCore::GetVSyncWait() { return this->vsyncWait; }
ID3D11RenderTargetView* D3DCore::GetDefaultRenderTargetView(){ return this->rtv; }
ID3D11DepthStencilView* D3DCore::GetDefaultDepthStencilView(){ return this->dsv; }


void D3DCore::SetDefaultRenderTarget() {
	cxt->OMSetRenderTargets(1, &this->rtv, dsv);
}

void D3DCore::SetViewport(D3D11_VIEWPORT* viewport){
	cxt->RSSetViewports(1, viewport);
}


void D3DCore::SetDefaultViewport(){
	cxt->RSSetViewports(1, &this->viewport);
}

void D3DCore::GetDefaultViewport(D3D11_VIEWPORT* viewport){
	*viewport = this->viewport;
}

void D3DCore::SetBlendMode(D3DBlendMode mode) {
	float zero[4] = { 0, 0, 0, 0 };
	switch (mode) {
		case D3DBlendMode::Add:
			cxt->OMSetBlendState(bsAdd, zero, 0xffffffff);
			break;
		case D3DBlendMode::Alpha:
			cxt->OMSetBlendState(bsAlpha, zero, 0xffffffff);
			break;
		default:
			cxt->OMSetBlendState(nullptr, zero, 0xffffffff);
			break;
	}
}


DWORD D3DCore::GetScreenWidth(){
	return wnd->GetWidth();
}

DWORD D3DCore::GetScreenHeight(){
	return wnd->GetHeight();
}

double D3DCore::GetRefreshRate() {
	BOOL fullscreen;
	swapChain->GetFullscreenState(&fullscreen, nullptr);

	if (fullscreen) {
		DXGI_SWAP_CHAIN_DESC scd;
		swapChain->GetDesc(&scd);
		if (scd.BufferDesc.RefreshRate.Denominator == 0) {
			return -1;
		}
		else {
			return (double)scd.BufferDesc.RefreshRate.Numerator / scd.BufferDesc.RefreshRate.Denominator;
		}
	}
	else {
		DWM_TIMING_INFO timingInfo = { 0 };
		timingInfo.cbSize = sizeof(DWM_TIMING_INFO);
		HWND hwnd = 0;

		/*
		** MSDNによれば、DwmGetComositionTimingInfoの第一引数(HWND)は
		** 「Windows8.1からはNULLを指定しないとE_INVALIDARGが起きるよ」とある。
		** ( http://msdn.microsoft.com/en-us/library/windows/desktop/aa969514%28v=vs.85%29.aspx )
		**
		** ただ、Windows Vista Professionalでも、ここをNULLにしないと動作しない、という報告もある。
		** ( http://rarara.cafe.coocan.jp/cgi-bin/lng/vc/vclng.cgi?print+201001/10010013.txt )
		**
		** 今回は、OSに限らず必ずNULL(0)を指定するようにする。
		** 今後、もしそれで問題が出るようだったら、以下のコメント部分にあるようなOSバージョン判定を行い
		** 適切な値を設定するように修正すること。
		*/

		//{
		//	OSVERSIONINFOEX ver;
		//	DWORDLONG dwlConditionMask = 0;
		//	ZeroMemory(&ver, sizeof(OSVERSIONINFOEX));
		//	ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		//	ver.dwMajorVersion = 6;
		//	ver.dwMinorVersion = 3;
		//	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_LESS);
		//	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_LESS);
		//	if (VerifyVersionInfo(&ver, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask)) {
		//		// windows 8.1 未満
		//		hwnd = wnd->GetWindowHandle();
		//	}
		//}

		switch (HRESULT hr = DwmGetCompositionTimingInfo(hwnd, &timingInfo)) {
			case S_OK:
				return timingInfo.rateRefresh.uiNumerator / (double)timingInfo.rateRefresh.uiDenominator;
			default:
				LOG_ERR("DwmGetCompositionTimingInfo failed (returns 0x%08X)\n", hr);
				return -1;
		}
	}
}

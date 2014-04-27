#include "D3DCore.h"
#include "DirectXUtil.h"

#include "Debug.h"

D3DCore::D3DCore(GameWindow* wnd)
{
	this->wnd = wnd;

	// �E�B���h�E�폜���ɂ͎��������ʂ悤�ɂ���
	wnd->AddResource(PtrToRes(this));
}


D3DCore::~D3DCore(void)
{
	if (!isDisposed()) Dispose();
}

bool D3DCore::Initialize(bool debugDevice) {
	HRESULT hr;

	// �X���b�v�`�F�C���̐ݒ�
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

	//D3D_FEATURE_LEVEL flevels[] = { D3D_FEATURE_LEVEL_10_1 };
	D3D_FEATURE_LEVEL flevels[] = { D3D_FEATURE_LEVEL_11_0 };

	// �f�o�C�X�ƃX���b�v�`�F�C���̍쐬
	IF_NG2(D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
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
			// ���s


			return false;
	}

	NameToResource(device, "MainDevice");
	auto dev = HndToRes(device);
	AddResource(dev);

	NameToResource(cxt, "MainDeviceContext");
	dev->AddResource(HndToRes(cxt));
	NameToResource(swapChain, "MainSwapchain");
	dev->AddResource(HndToRes(swapChain));

	

	// �o�b�N�o�b�t�@�̍쐬

	ID3D11Texture2D *backbuffer;
	IF_NG2(swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D), 
		(void**)&backbuffer) , hr) {

			// ���s
			return false;
	}
	IF_NG2(device->CreateRenderTargetView(backbuffer, NULL, &rtv), hr){
			// ���s
			return false;
	}
	NameToResource(rtv, "DefaultRenderTargetView");
	dev->AddResource(HndToRes(rtv));

	// �[�x�o�b�t�@�̍쐬
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

	// �o�b�N�o�b�t�@�Ɛ[�x�o�b�t�@��ݒ�
	cxt->OMSetRenderTargets(1, &rtv, dsv);
	IF_NG2(backbuffer->Release(), hr){
			// ���s
			return false;
	};

	// �r���[�|�[�g�̍쐬
	viewport.Width = (float)wnd->GetWidth();
	viewport.Height = (float)wnd->GetHeight();
	viewport.TopLeftX = viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	cxt->RSSetViewports(1, &viewport);


	// �u�����h�X�e�[�g
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
		tg->DestBlendAlpha = D3D11_BLEND_ZERO;

		tg->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	IF_NG2(device->CreateBlendState(&bd, &this->bs), hr){
		return false;
	}
	NameToResource(bs, "DefaultBlendState");
	dev->AddResource(HndToRes(bs));
	float zero[4] = { 0, 0, 0, 0 };
	cxt->OMSetBlendState(bs, zero, 0xffffffff);


	// ��ʏ���������Ƃ��̐F
	this->clearColor[0] = 0.0f;
	this->clearColor[1] = 0.0f;
	this->clearColor[2] = 0.0f;
	this->clearColor[3] = 1.0f;

	// �����X�e�[�g�̐ݒ�
	this->SetDefaultRenderTarget();
	this->SetDefaultViewport();

	// VSync�҂�(�f�t�H���g�͑҂��Ȃ�)
	this->vsyncWait = 0;

	return true;
	
}

bool D3DCore::isDisposed() {
	return device == nullptr;
}

void D3DCore::Dispose() {
	if(isDisposed()) return;
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

void D3DCore::DrawIndexed(int indexCount, int vertexOffset, int indexOffset){
	cxt->DrawIndexed(indexCount, indexOffset, vertexOffset);
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




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
		0,
		nullptr, 0,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&device,
		flevels,
		&cxt), hr) {
			// ���s


			return false;
	}

	auto dev = HndToRes(device);
	AddResource(dev);
	AddResource(HndToRes(swapChain));

	

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
	dev->AddResource(HndToRes(depthStencil));

	IF_NG2(device->CreateDepthStencilView(this->depthStencil, nullptr, &this->dsv), hr){
		return false;
	}
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


	// ��ʏ���������Ƃ��̐F
	this->clearColor[0] = 0.0f;
	this->clearColor[1] = 0.0f;
	this->clearColor[2] = 0.0f;
	this->clearColor[3] = 1.0f;

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
	Resource::Dispose();
	device = nullptr;
}

void D3DCore::Clear() {
	cxt->ClearRenderTargetView(rtv, clearColor);
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

void D3DCore::SetDefaultRenderTarget() {
	cxt->OMSetRenderTargets(1, &this->rtv, dsv);
	
}




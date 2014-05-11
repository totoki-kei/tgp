#include "D3DTexture.h"

D3DTexture2D::D3DTexture2D(D3DCore *core, int width, int height, DXGI_FORMAT format, UINT bind, UINT misc)
: base_t(core)
{
	D3D11_TEXTURE2D_DESC desc;

	// data unit
	desc.Format = format;
	desc.Height = height;
	desc.Width = width;

	// sampling
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	// array, mipmap
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.MiscFlags = misc;

	// access
	desc.BindFlags = bind;
	//D3D11_BIND_SHADER_RESOURCE
	//	| (renderable ? D3D11_BIND_RENDER_TARGET : 0U);
	desc.CPUAccessFlags = 0U;
	desc.Usage = D3D11_USAGE_DEFAULT;

	auto device = core->GetDevice();
	auto hresult = device->CreateTexture2D(&desc, nullptr, &this->texture);

	this->AddResource(HndToRes(texture));

	if (bind & D3D10_BIND_SHADER_RESOURCE) { // ShaderResourceView�̍쐬
		device->CreateShaderResourceView(texture, nullptr, &srv);

		this->AddResource(HndToRes(srv));
	}
	else {
		srv = nullptr;
	}

	if (bind & D3D10_BIND_RENDER_TARGET) { // RenderTargetView�̍쐬
		device->CreateRenderTargetView(texture, nullptr, &rtv);

		this->AddResource(HndToRes(rtv));
	}
	else {
		rtv = nullptr;
	}

	if (bind & D3D10_BIND_DEPTH_STENCIL) { // DepthStencilView�̍쐬
		device->CreateDepthStencilView(texture, nullptr, &dsv);

		this->AddResource(HndToRes(dsv));
	}
	else {
		dsv = nullptr;
	}

}

void D3DTexture2D::SetToRenderTarget(){
	auto context = core->GetDeviceContext();
	context->OMSetRenderTargets(1, &this->rtv, nullptr);
}

void D3DTexture2D::SetToDepthStencil(){
	auto context = core->GetDeviceContext();
	ID3D11RenderTargetView* rtvs[] = { core->GetDefaultRenderTargetView() };
	context->OMSetRenderTargets(1, rtvs, this->dsv);
}

void D3DTexture2D::SetToRenderTargetAndDepth(D3DTexture2D *depth){
	auto context = core->GetDeviceContext();
	context->OMSetRenderTargets(1, &this->rtv, depth->dsv);
}

void D3DTexture2D::ClearAsRenderTarget(XMFLOAT4 color){
	FLOAT f[] = { color.x, color.y, color.z, color.w };
	core->GetDeviceContext()->ClearRenderTargetView(rtv, f);
}
void D3DTexture2D::ClearAsDepthStencil(){
	core->GetDeviceContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

}

void D3DTexture2D::DrawAsDc(std::function<void (const HDC, RECT**)> fn){
	IDXGISurface1* surface = nullptr;
	HRESULT hr = texture->QueryInterface(__uuidof(IDXGISurface1), (void**)&surface);
	IF_NG(hr) {
		DBG_OUT("Failed to get IDXGISurface1 Interface.\r\n");
		return;
	}

	HDC dc = nullptr;
	hr = surface->GetDC(false, &dc);
	IF_NG(hr) {
		DBG_OUT("Failed to get HDC Handle.\r\n");
		surface->Release();
		return;
	}

	RECT* prect;

	fn(dc, &prect);

	surface->ReleaseDC(prect);

	if (prect) delete prect;
	surface->Release();

}



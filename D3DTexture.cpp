#include "D3DTexture.h"

D3DTexture2D::D3DTexture2D(D3DCore *core, int width, int height, DXGI_FORMAT format, D3D11_BIND_FLAG bind)
: base_t(core)
{
	D3D11_TEXTURE2D_DESC desc;

	// data unit
	desc.Format = format;
	desc.Height = height;
	desc.Width = width;

	// sampling
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 1;

	// array, mipmap
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.MiscFlags = 0;

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
	ID3D11DepthStencilView* dsvs;
	context->OMGetRenderTargets(1, nullptr, &dsvs);
	context->OMSetRenderTargets(1, &this->rtv, dsvs);
}

void D3DTexture2D::SetToDepthStencil(){
	auto context = core->GetDeviceContext();
	ID3D11RenderTargetView* rtvs;
	context->OMGetRenderTargets(1, &rtvs, nullptr);
	context->OMSetRenderTargets(1, &rtvs, this->dsv);
}

void D3DTexture2D::SetToRenderTargetAndDepth(D3DTexture2D *depth){
	auto context = core->GetDeviceContext();
	context->OMSetRenderTargets(1, &this->rtv, depth->dsv);
}

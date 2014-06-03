#include "D3DTexture.h"

void D3DTexture2D::Initialize(int width, int height, DXGI_FORMAT format, UINT bind, UINT misc, const D3D11_SUBRESOURCE_DATA* data)
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
	auto hresult = device->CreateTexture2D(&desc, data, &this->texture);

	this->AddResource(HndToRes(texture));

	if (bind & D3D10_BIND_SHADER_RESOURCE) { // ShaderResourceView‚Ìì¬
		device->CreateShaderResourceView(texture, nullptr, &srv);

		this->AddResource(HndToRes(srv));
	}
	else {
		srv = nullptr;
	}

	if (bind & D3D10_BIND_RENDER_TARGET) { // RenderTargetView‚Ìì¬
		device->CreateRenderTargetView(texture, nullptr, &rtv);

		this->AddResource(HndToRes(rtv));
	}
	else {
		rtv = nullptr;
	}

	if (bind & D3D10_BIND_DEPTH_STENCIL) { // DepthStencilView‚Ìì¬
		device->CreateDepthStencilView(texture, nullptr, &dsv);

		this->AddResource(HndToRes(dsv));
	}
	else {
		dsv = nullptr;
	}

}

D3DTexture2D::D3DTexture2D(D3DCore *core, int width, int height, DXGI_FORMAT format, UINT bind, UINT misc) : base_t(core)
{
	Initialize(width, height, format, bind, misc);
}

D3DTexture2D::D3DTexture2D(D3DCore *core, int width, int height,
	DXGI_FORMAT format, D3DTextureUsage usage, bool gdiCompatible) : base_t{ core }{

	UINT bind;
	switch (usage){
	case D3DTextureUsage::RenderTarget:
		bind = D3D10_BIND_SHADER_RESOURCE;
		break;
	case D3DTextureUsage::ShaderResource:
		bind = D3D11_BIND_SHADER_RESOURCE;
		break;
	case D3DTextureUsage::Both:
		bind = D3D10_BIND_SHADER_RESOURCE | D3D11_BIND_SHADER_RESOURCE;
		break;
	}

	UINT misc = 0U;
	if (gdiCompatible){
		if (format != DXGI_FORMAT_B8G8R8A8_UNORM && format != DXGI_FORMAT_B8G8R8A8_UNORM_SRGB){
			// GDI_COMPATIBLE‚Ì—vŒ‚ð–ž‚½‚µ‚Ä‚¢‚È‚¢
			DBG_OUT("Bad format for GdiCompatible option.");
			return;
		}

		misc = D3D11_RESOURCE_MISC_GDI_COMPATIBLE;
	}

	Initialize(width, height, format, bind, misc);
}

D3DTexture2D::D3DTexture2D(D3DCore *core, ImageData* image, D3DTextureUsage usage, bool gdiCompatible) : base_t{ core }{
	UINT bind;
	int width = image->GetWidth();
	int height = image->GetHeight();
	DXGI_FORMAT format = image->GetFormat();

	switch (usage){
	case D3DTextureUsage::RenderTarget:
		bind = D3D10_BIND_SHADER_RESOURCE;
		break;
	case D3DTextureUsage::ShaderResource:
		bind = D3D11_BIND_SHADER_RESOURCE;
		break;
	case D3DTextureUsage::Both:
		bind = D3D10_BIND_SHADER_RESOURCE | D3D11_BIND_SHADER_RESOURCE;
		break;
	}

	UINT misc = 0U;
	if (gdiCompatible){
		if (format != DXGI_FORMAT_B8G8R8A8_UNORM && format != DXGI_FORMAT_B8G8R8A8_UNORM_SRGB){
			// GDI_COMPATIBLE‚Ì—vŒ‚ð–ž‚½‚µ‚Ä‚¢‚È‚¢
			DBG_OUT("Bad format for GdiCompatible option.");
			return;
		}


		misc = D3D11_RESOURCE_MISC_GDI_COMPATIBLE;
	}

	D3D11_SUBRESOURCE_DATA subres;
	subres.pSysMem = image->GetData();
	subres.SysMemPitch = image->GetStride();
	subres.SysMemSlicePitch = 0;

	Initialize(width, height, format, bind, misc, &subres);

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



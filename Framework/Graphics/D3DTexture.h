#pragma once

#include "D3DCore.h"

#include "D3DShader.h"
#include "../Resource.h"

#include "ImageLoader.h"

template <typename TextureT, typename ResourceT>
class D3DTextureBase : public Resource
{
protected:
	ResourceT* texture;
	ID3D11ShaderResourceView* srv;
	D3DCore * core;
public:
	D3DTextureBase(D3DCore * core){
		this->core = core;
		this->texture = nullptr;
	}
	virtual ~D3DTextureBase(){
		if (!isDisposed()) Dispose();
	}

	void Apply(Shaders::ShaderFlag targetShader, int index){
		if (!srv) {
			DBG_OUT("shader resour view is null.");
			return;
		}

		auto ctx = core->GetDeviceContext();
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Vertex)){
			ctx->VSSetShaderResources(index, 1, &this->srv);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Pixel)){
			ctx->PSSetShaderResources(index, 1, &this->srv);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Geometry)){
			ctx->GSSetShaderResources(index, 1, &this->srv);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Compute)){
			ctx->PSSetShaderResources(index, 1, &this->srv);
		}
	}

	void Unapply(Shaders::ShaderFlag targetShader, int index){
		auto ctx = core->GetDeviceContext();

		ID3D11ShaderResourceView* nullarray[] = { nullptr };

		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Vertex)){
			ctx->VSSetShaderResources(index, 1, nullarray);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Pixel)){
			ctx->PSSetShaderResources(index, 1, nullarray);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Geometry)){
			ctx->GSSetShaderResources(index, 1, nullarray);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Compute)){
			ctx->PSSetShaderResources(index, 1, nullarray);
		}
	}

};


enum class D3DTextureUsage {
	ShaderResource,
	RenderTarget,
	Both,
};

class D3DTexture2D : public D3DTextureBase<D3DTexture2D, ID3D11Texture2D> {
protected:
	void Initialize(int width, int height,
		DXGI_FORMAT format, UINT bind = D3D11_BIND_SHADER_RESOURCE, UINT misc = 0U, const D3D11_SUBRESOURCE_DATA* data = nullptr);
public:
	typedef D3DTextureBase<D3DTexture2D, ID3D11Texture2D> base_t;

	ID3D11RenderTargetView* rtv;
	ID3D11DepthStencilView* dsv;

	D3DTexture2D(D3DCore *core, int width, int height,
		DXGI_FORMAT format, UINT bind = D3D11_BIND_SHADER_RESOURCE, UINT misc = 0U);

	D3DTexture2D(D3DCore *core, int width, int height,
		DXGI_FORMAT format, D3DTextureUsage usage = D3DTextureUsage::ShaderResource, bool gdiCompatible = false);
	D3DTexture2D(D3DCore *core, ImageData* image, D3DTextureUsage usage = D3DTextureUsage::ShaderResource, bool gdiCompatible = false);


	void SetToRenderTarget();
	void SetToDepthStencil();
	void SetToRenderTargetAndDepth(D3DTexture2D *depth);

	void ClearAsRenderTarget(XMFLOAT4 color);
	void ClearAsDepthStencil();

	void DrawAsDc(std::function<void(const HDC, RECT**)> fn);

};


#include "D3DBloomEffect.h"


D3DBloomEffect::D3DBloomEffect(D3DCore* c, unsigned int width, unsigned int height) {
	this->core = c;
	offscreenSurface.reset(new D3DTexture2D(core, width, height, DXGI_FORMAT_R32G32B32A32_FLOAT, D3DTextureUsage::Both));

	sprite.reset(new Sprite::Sprite(nullptr));

	for (int i = 0; i < DOWNSCALE_COUNT; i++) {
		int w = width  >> (i + 1);
		int h = height >> (i + 1);
		downscaleSurface[i].reset(new D3DTexture2D(core, w, h, DXGI_FORMAT_R8G8B8A8_UNORM, D3DTextureUsage::Both));
	}

	gaussianSurface.reset(new D3DTexture2D(core, width >> DOWNSCALE_COUNT, height >> DOWNSCALE_COUNT, DXGI_FORMAT_R8G8B8A8_UNORM, D3DTextureUsage::Both));

	filter.reset(new Sprite::BrightPassEffector(core));

	gauss_h.reset( new Sprite::GaussianEffector(core, Sprite::GAUSS_HORIZONTAL));
	gauss_v.reset( new Sprite::GaussianEffector(core, Sprite::GAUSS_VERTICAL));

	D3D11_SAMPLER_DESC smpdesc;
	D3DSampler::GetDefaultSamplerDesc(&smpdesc);
	smpdesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	smpdesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	smpdesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerForGauss.reset(new D3DSampler(core, &smpdesc));
	NameToResource(samplerForGauss->GetRawSampler(), "SamplerForGauss");

	threshold = 0.5f;
}


D3DBloomEffect::~D3DBloomEffect() {}

void D3DBloomEffect::Preprocess() {
	// (1) オフスクリーンバッファへ描画
	offscreenSurface->SetToRenderTarget();
	offscreenSurface->ClearAsRenderTarget(DirectX::XMFLOAT4(0, 0, 0, 0));
	core->ClearDepth();
}

void D3DBloomEffect::Postprocess() {


	// (2) ダウンスケール
	sprite->SetTexture(offscreenSurface.get());
	sprite->SetEffector(filter.get()); // 初回のみハイパスフィルタを通す
	filter->SetThreshold(threshold);
	for (int i = 0; i < DOWNSCALE_COUNT; i++) {
		auto suf = downscaleSurface[i].get();
		suf->Unapply(Shaders::ShaderFlag::Pixel, 0);
		suf->SetToRenderTargetAndDepth(nullptr);
		suf->ClearAsRenderTarget(DirectX::XMFLOAT4(0, 0, 0, 0));

		sprite->DrawQuad(0, 0, (float)suf->GetWidth(), (float)suf->GetHeight());
		sprite->Flush();

		sprite->SetTexture(suf);
		sprite->SetEffector(nullptr); // 二回目以降は通常のエフェクタにする
	}

	// (3) ガウスフィルタ
	sprite->SetSampler(samplerForGauss.get());
	// (3)-1 横方向
	gaussianSurface->Unapply(Shaders::ShaderFlag::Pixel, 0);
	gaussianSurface->SetToRenderTargetAndDepth(nullptr);
	gaussianSurface->ClearAsRenderTarget(DirectX::XMFLOAT4(0, 0, 0, 0));
	sprite->SetEffector(gauss_h.get());
	sprite->DrawQuad(0, 0, (float)gaussianSurface->GetWidth(), (float)gaussianSurface->GetHeight());
	sprite->Flush();
	sprite->SetTexture(gaussianSurface.get());
	// (3)-2 縦方向
	downscaleSurface[DOWNSCALE_COUNT - 1]->Unapply(Shaders::ShaderFlag::Pixel, 0);
	downscaleSurface[DOWNSCALE_COUNT - 1]->SetToRenderTargetAndDepth(nullptr);
	downscaleSurface[DOWNSCALE_COUNT - 1]->ClearAsRenderTarget(DirectX::XMFLOAT4(0, 0, 0, 0));
	sprite->SetEffector(gauss_v.get());
	sprite->DrawQuad(0, 0, (float)downscaleSurface[DOWNSCALE_COUNT - 1]->GetWidth(), (float)downscaleSurface[DOWNSCALE_COUNT - 1]->GetHeight());
	sprite->Flush();

	// (4) バックバッファへ
	core->SetDefaultRenderTarget();
	core->ClearDepth();

	sprite->SetEffector(nullptr);
	sprite->SetTexture(offscreenSurface.get());
	sprite->DrawQuad();
	sprite->Flush();

	core->SetBlendMode(D3DBlendMode::Add);
	//sprite->SetEffector(filter);
	//filter->SetParameter(0, 0.75, 0, 1);
	sprite->SetTexture(downscaleSurface[DOWNSCALE_COUNT - 1].get());
	sprite->DrawQuad();
	sprite->Flush();

	core->SetBlendMode(D3DBlendMode::Alpha);

}

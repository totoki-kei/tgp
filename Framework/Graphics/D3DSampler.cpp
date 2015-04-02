#include "D3DSampler.h"


D3DSampler::D3DSampler(D3DCore *core)
: core(core)
{
	D3D11_SAMPLER_DESC desc;
	GetDefaultSamplerDesc(&desc);
	core->GetDevice()->CreateSamplerState(&desc, &sampler);

	this->AddResource(HndToRes(sampler));
}

D3DSampler::D3DSampler(D3DCore *core, D3D11_SAMPLER_DESC* desc)
: core(core)
{
	core->GetDevice()->CreateSamplerState(desc, &sampler);
	this->AddResource(HndToRes(sampler));
}

void D3DSampler::GetDefaultSamplerDesc(D3D11_SAMPLER_DESC* desc){

	desc->AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc->AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc->AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	desc->BorderColor[0] = 0.0f;
	desc->BorderColor[1] = 0.0f;
	desc->BorderColor[2] = 0.0f;
	desc->BorderColor[3] = 0.0f;

	desc->ComparisonFunc = D3D11_COMPARISON_ALWAYS;

	desc->Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc->MaxAnisotropy = 1;

	desc->MipLODBias = 0.0f;
	desc->MinLOD = 0.0f;
	desc->MaxLOD = D3D10_FLOAT32_MAX;
}

D3DSampler::~D3DSampler(){
	if (!isDisposed())Dispose();
}



void D3DSampler::Apply(Shaders::ShaderFlag targetShader, int index) {
	auto ctx = core->GetDeviceContext();

	if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Vertex)){
		ctx->VSSetSamplers(index, 1, &this->sampler);
	}
	if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Pixel)){
		ctx->PSSetSamplers(index, 1, &this->sampler);
	}
	if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Geometry)){
		ctx->GSSetSamplers(index, 1, &this->sampler);
	}
	if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Compute)){
		ctx->CSSetSamplers(index, 1, &this->sampler);
	}
}

void D3DSampler::Unapply(Shaders::ShaderFlag targetShader, int index) {
	auto ctx = core->GetDeviceContext();

	ID3D11SamplerState* nullsampler[] = { nullptr };

	if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Vertex)){
		ctx->VSSetSamplers(index, 1, nullsampler);
	}
	if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Pixel)){
		ctx->PSSetSamplers(index, 1, nullsampler);
	}
	if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Geometry)){
		ctx->GSSetSamplers(index, 1, nullsampler);
	}
	if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Compute)){
		ctx->PSSetSamplers(index, 1, nullsampler);
	}
}

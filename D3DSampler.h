#pragma once
#include "D3DCore.h"
#include "D3DShader.h"
#include "Resource.h"
#include "DirectXUtil.h"

class D3DSampler :
	public Resource
{
	D3DCore *core;
	ID3D11SamplerState* sampler;
public:
	static void GetDefaultSamplerDesc(D3D11_SAMPLER_DESC* desc);

	D3DSampler(D3DCore *core);
	D3DSampler(D3DCore *core, D3D11_SAMPLER_DESC* desc);
	~D3DSampler();

	//bool isDisposed();
	//void Dispose();

	void Apply(Shaders::ShaderFlag targetShader, int index);
	void Unapply(Shaders::ShaderFlag targetShader, int index);
};


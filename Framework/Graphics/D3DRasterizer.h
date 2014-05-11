#pragma once
#include "../Resource.h"
#include "D3DCore.h"

class D3DRasterizer :
	public Resource
{
	D3DCore *core;
	ID3D11RasterizerState* rs;
	
public:

	D3D11_RECT scissor;
	D3D11_VIEWPORT viewport;

	D3DRasterizer(D3DCore* core);
	D3DRasterizer(D3DCore* core, D3D11_RASTERIZER_DESC *desc);
	~D3DRasterizer();

	static void GetDefaultRasterizerDesc(D3D11_RASTERIZER_DESC *desc);

	void Apply();
	void Unapply();
};


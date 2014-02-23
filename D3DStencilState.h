#pragma once
#include "D3DCore.h"
#include "Resource.h"
#include "DirectXUtil.h"

class D3DStencilState :
	public Resource
{
	D3DCore *core;
	ID3D11DepthStencilState* dss;
public:
	static void GetDefaultDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC* desc);

	D3DStencilState(D3DCore *core, bool depthEnable = true, bool stencilEnable = false);
	D3DStencilState(D3DCore *core, D3D11_DEPTH_STENCIL_DESC* desc);
	~D3DStencilState();


	bool isDisposed();
	//void Dispose();

	void Apply();
	void Unapply();

};


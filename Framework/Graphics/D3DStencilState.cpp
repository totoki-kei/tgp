#include "D3DStencilState.h"


D3DStencilState::D3DStencilState(D3DCore *core, bool depthEnable, bool stencilEnable)
: core(core)
{
	D3D11_DEPTH_STENCIL_DESC desc;
	GetDefaultDepthStencilDesc(&desc);
	desc.DepthEnable = depthEnable;
	desc.StencilEnable = stencilEnable;
	core->GetDevice()->CreateDepthStencilState(&desc, &dss);
	this->AddResource(HndToRes(dss));
}

D3DStencilState::D3DStencilState(D3DCore *core, D3D11_DEPTH_STENCIL_DESC* desc)
: core(core)
{
	core->GetDevice()->CreateDepthStencilState(desc, &dss);
	this->AddResource(HndToRes(dss));
}

void D3DStencilState::GetDefaultDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC* desc){
	desc->DepthEnable = true;
	desc->DepthFunc = D3D11_COMPARISON_LESS;
	desc->DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	desc->StencilEnable = false;
	desc->StencilReadMask = 0xff;
	desc->StencilWriteMask = 0xff;

	desc->FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc->FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	desc->FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc->FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	desc->BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc->BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	desc->BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc->BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
}


D3DStencilState::~D3DStencilState()
{
	if (!isDisposed())Dispose();
}


void D3DStencilState::Apply(){
	core->GetDeviceContext()->OMSetDepthStencilState(dss, 0);
}

void D3DStencilState::Unapply(){
	core->GetDeviceContext()->OMSetDepthStencilState(nullptr, 0);
}

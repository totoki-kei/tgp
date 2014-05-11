#include "D3DRasterizer.h"

#include "../DirectXUtil.h"


D3DRasterizer::D3DRasterizer(D3DCore* core)
{
	D3D11_RASTERIZER_DESC desc;
	GetDefaultRasterizerDesc(&desc);

	core->GetDevice()->CreateRasterizerState(&desc, &this->rs);
	this->AddResource(HndToRes(rs));
}

D3DRasterizer::D3DRasterizer(D3DCore* core, D3D11_RASTERIZER_DESC *desc)
{

	core->GetDevice()->CreateRasterizerState(desc, &this->rs);
	this->AddResource(HndToRes(rs));
}

void D3DRasterizer::GetDefaultRasterizerDesc(D3D11_RASTERIZER_DESC *desc){
	desc->FillMode = D3D11_FILL_SOLID;
	desc->CullMode = D3D11_CULL_BACK;
	desc->FrontCounterClockwise = false;
	desc->DepthBias = 0;
	desc->DepthBiasClamp = 0;
	desc->SlopeScaledDepthBias = 0;
	desc->DepthClipEnable = true;
	desc->ScissorEnable = false;
	desc->MultisampleEnable = false;
	desc->AntialiasedLineEnable = false;
}


D3DRasterizer::~D3DRasterizer()
{

}


void D3DRasterizer::Apply(){
	auto context = core->GetDeviceContext();
	context->RSSetState(rs);
	context->RSSetScissorRects(1, &scissor);
	context->RSSetViewports(1, &viewport);
}

void D3DRasterizer::Unapply(){
	auto context = core->GetDeviceContext();
	context->RSSetState(nullptr);
}
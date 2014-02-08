#pragma once

// DXGIŠÖŒW‚Ì‚à‚Ì‚ªÄ’è‹`‚³‚ê‚é‚½‚ßAˆê•”Œx‚ğ–³Œø‚É
#include "D3DHeaders.h"
#include "D3DLibs.h"
#include "D3DStruct.h"

#include "Resource.h"
#include "GameWindow.h"

#include "D3DEnum.h"

class D3DCore : public Resource
{
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* cxt;
	ID3D11RenderTargetView* rtv;
	D3D11_VIEWPORT viewport;

	GameWindow* wnd;

	D3DFloats<4> clearColor;
public:
	D3DCore(GameWindow* wnd);
	~D3DCore(void);

	bool Initialize();

	bool isDisposed();
	void Dispose();

	void Clear();
	void Update();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;


public:
	void SetPrimitiveTopology(D3DPrimitiveTopology topology);

};


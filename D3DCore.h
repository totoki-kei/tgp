#pragma once

// DXGIŠÖŒW‚Ì‚à‚Ì‚ªÄ’è‹`‚³‚ê‚é‚½‚ßAˆê•”Œx‚ğ–³Œø‚É
#include "D3DHeaders.h"
#include "D3DLibs.h"

#include "Resource.h"
#include "GameWindow.h"

#include "D3DEnum.h"

class D3DCore : public Resource
{
	IDXGISwapChain* swapChain;

	ID3D10Device* device;
	ID3D10RenderTargetView* rtv;
	D3D10_VIEWPORT viewport;

	GameWindow* wnd;

	D3DXCOLOR clearColor;
public:
	D3DCore(GameWindow* wnd);
	~D3DCore(void);

	bool Initialize();

	bool isDisposed();
	void Dispose();

	void Clear();
	void Update();

	ID3D10Device* GetDevice() const;

public:
	void SetPrimitiveTopology(D3DPrimitiveTopology topology);

};


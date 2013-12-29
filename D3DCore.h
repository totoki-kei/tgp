#pragma once

// DXGIŠÖŒW‚Ì‚à‚Ì‚ªÄ’è‹`‚³‚ê‚é‚½‚ßAˆê•”Œx‚ğ–³Œø‚É
#pragma warning(push)
#pragma warning(disable:4005)
#include <d3d10.h>
#include <DXGI.h>
#include <D3DX10.h>
#include <DxErr.h>
#pragma warning(pop)

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d10.lib")
#if _DEBUG
#pragma comment(lib, "d3dx10d.lib")
#else
#pragma comment(lib, "d3dx10.lib")
#endif
#pragma comment(lib, "dxerr.lib")

#include "Resource.h"
#include "GameWindow.h"

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

};


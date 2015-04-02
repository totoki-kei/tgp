#pragma once

class D3DCore;

// DXGIä÷åWÇÃÇ‡ÇÃÇ™çƒíËã`Ç≥ÇÍÇÈÇΩÇﬂÅAàÍïîåxçêÇñ≥å¯Ç…
#include "D3DHeaders.h"

#include "../Resource.h"
#include "../GameWindow.h"

#include "D3DEnum.h"

class D3DCore : public Resource
{
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* cxt;
	ID3D11RenderTargetView* rtv;

	ID3D11Texture2D* depthStencil;
	ID3D11DepthStencilView* dsv;
	ID3D11DepthStencilState* dss;

	ID3D11BlendState* bsNormal;
	ID3D11BlendState* bsAlpha;
	ID3D11BlendState* bsAdd;

	D3D11_VIEWPORT viewport;

	GameWindow* wnd;

	float clearColor[4];
	int vsyncWait;

	ID3D11Debug * d3ddbg;

public:
	D3DCore(GameWindow* wnd);
	~D3DCore(void);

	bool Initialize(bool fullscreen = false, bool debugDevice = false);

	bool isDisposed();
	void Dispose();

	void Clear();
	void Update();

	void SetClearColor(XMFLOAT4 color);
	void ClearRenderTarget();
	void ClearRenderTarget(XMFLOAT4 color);
	void ClearDepth();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;


public:
	void SetPrimitiveTopology(D3DPrimitiveTopology topology);

	void Draw(int vertexCount, int offset);
	void DrawIndexed(int indexCount, int vertexOffset, int indexOffset);
	void DrawIndexed(int indexCount, int vertexOffset, int indexOffset, int instanceCount);

	void SetVSyncWait(int wait);
	int GetVSyncWait();

	ID3D11RenderTargetView* GetDefaultRenderTargetView();
	ID3D11DepthStencilView* GetDefaultDepthStencilView();
	void SetDefaultRenderTarget();

	void SetViewport(D3D11_VIEWPORT* viewport);

	void SetDefaultViewport();
	void GetDefaultViewport(D3D11_VIEWPORT* viewport);

	void SetBlendMode(D3DBlendMode mode);

	DWORD GetScreenWidth();
	DWORD GetScreenHeight();
	double GetRefreshRate();
};


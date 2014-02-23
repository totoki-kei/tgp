#pragma once

class D3DCore;

// DXGI関係のものが再定義されるため、一部警告を無効に
#include "D3DHeaders.h"
#include "D3DLibs.h"

#include "Resource.h"
#include "GameWindow.h"

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
	D3D11_VIEWPORT viewport;

	GameWindow* wnd;

	float clearColor[4];
	int vsyncWait;
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

	void Draw(int vertexCount, int offset);
	void DrawIndexed(int indexCount, int vertexOffset, int indexOffset);

	void SetVSyncWait(int wait);
	int GetVSyncWait();

	void SetDefaultRenderTarget();
};


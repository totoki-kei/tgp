#pragma once
#include "D3DBuffer.h"

template <typename IndexT = unsigned short, int Format = DXGI_FORMAT::DXGI_FORMAT_R16_UINT>
class D3DIndexBuffer : public D3DBuffer {
	size_t length;
public:
	typedef IndexT index_t;

	D3DIndexBuffer(D3DCore *core, size_t length) : D3DBuffer(core) {
		this->length = length;
		InitializeBuffer(nullptr, sizeof(IndexT) * length, D3D11_BIND_INDEX_BUFFER, false);
	}

	D3DIndexBuffer(D3DCore *core, const IndexT* data, size_t length, bool cpuAccessable = false) : D3DBuffer(core) {
		this->length = length;
		InitializeBuffer(data, sizeof(IndexT)* length, D3D11_BIND_INDEX_BUFFER, cpuAccessable);
	}

	template<size_t Length>
	D3DIndexBuffer(D3DCore* core, IndexT(&data)[Length], bool cpuAccessable = false) : D3DBuffer(core) {
		this->length = Length;
		InitializeBuffer(data, sizeof(IndexT)* length, D3D11_BIND_INDEX_BUFFER, cpuAccessable);
	}


	void Update(const IndexT* b) {
		UpdateBuffer(b, sizeof(IndexT)* length);
	}

	void Update(const IndexT* b, size_t offset, size_t length) {
		UpdateBuffer(b, sizeof(IndexT) * offset, sizeof(IndexT) * length);
	}

	void Apply() {
		auto ctx = core->GetDeviceContext();
		ctx->IASetIndexBuffer(buffer, (DXGI_FORMAT)Format, 0);
	}

	void Unapply() {
		auto ctx = core->GetDeviceContext();
		ctx->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	}

	static void Unapply(D3DCore *core) {
		auto ctx = core->GetDeviceContext();
		ctx->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	}


	size_t GetLength() { return length; }
};


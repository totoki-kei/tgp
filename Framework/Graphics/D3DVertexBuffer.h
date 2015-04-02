#pragma once
#include "D3DBuffer.h"

template <typename T>
class D3DVertexBuffer : public D3DBuffer {
	size_t length;
public:
	enum {
		Stride = sizeof(T),
	};
	typedef T vertex_t;

	D3DVertexBuffer(D3DCore *core, size_t length) : D3DBuffer(core) {
		this->length = length;
		InitializeBuffer(nullptr, Stride * length, D3D11_BIND_VERTEX_BUFFER, false);
	}

	D3DVertexBuffer(D3DCore *core, const T* data, size_t length, bool cpuAccessable = false) : D3DBuffer(core) {
		this->length = length;
		InitializeBuffer(data, Stride * length, D3D11_BIND_VERTEX_BUFFER, cpuAccessable);
	}

	template<size_t Length>
	D3DVertexBuffer(D3DCore* core, T(&data)[Length], bool cpuAccessable = false) : D3DBuffer(core) {
		this->length = Length;
		InitializeBuffer(data, Stride * length, D3D11_BIND_VERTEX_BUFFER, cpuAccessable);
	}

	void Update(const T* b) {
		UpdateBuffer(b, Stride * length);
	}

	void Update(const T* b, size_t offset, size_t length) {
		UpdateBuffer(b, Stride * offset, Stride * length);
	}

	void Apply(int slot = 0) {
		UINT stride[] = { Stride };
		UINT offset[] = { 0 };
		core->GetDeviceContext()->IASetVertexBuffers(slot, 1, &buffer, stride, offset);
	}

	template <typename ExtraBufferT>
	void Apply(const ExtraBufferT* exbuffer) {
		UINT stride[] = { Stride, exbuffer ? exbuffer->Stride : 0 };
		UINT offset[] = { 0 , 0 };
		ID3D11Buffer* buffers[] = { buffer, exbuffer ? exbuffer->GetRawBuffer() : nullptr };
		core->GetDeviceContext()->IASetVertexBuffers(0, 2, buffers, stride, offset);
	}

	static void Unapply() {
		UINT stride[] = { Stride };
		UINT offset[] = { 0 };
		core->GetDeviceContext()->IASetVertexBuffers(0, 0, nullptr, stride, offset);
	}

	size_t GetLength() { return length; }
};

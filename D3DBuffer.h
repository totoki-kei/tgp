#pragma once

#pragma warning(push)
#pragma warning(disable:4005)
#include <D3D10.h>
#pragma warning(pop)

class D3DBuffer;

#include "D3DCore.h"
#include "Resource.h"


class D3DBuffer :	public Resource {

private:

	D3DCore* core;
	ID3D10Buffer *buffer;
	//std::weak_ptr<BufferData> mapped;

protected:
	D3DBuffer(D3DCore*);
public:
	virtual ~D3DBuffer();

	void Dispose();

	//std::shared_ptr<BufferData> GetData();

protected:
	void InitializeBuffer(void* bufferData, int bufferSize, UINT usage, bool readonly = true);

};
/*
class BufferData {
	friend class D3DBuffer;
protected:
	D3DBuffer* buffer;
	void* data;


public:
	void* GetPointer() const;

	BufferData(){};
	virtual ~BufferData();
	void Release();
};
*/
//// ここから用途別に特殊化されたバッファ

template <typename T, int Length>
class D3DVertexBuffer : D3DBuffer {

public:
	enum {
		Stride = sizeof(T),
		TotalBytes = sizeof(T)*Length,
	};

	D3DVertexBuffer(D3DCore *core) : D3DBuffer(core) {
		InitializeBuffer(nullptr, TotalBytes, D3D10_BIND_VERTEX_BUFFER, false);
	}
	D3DVertexBuffer(D3DCore *core, const T* data) : D3DBuffer(core) {
		InitializeBuffer(this->data, TotalBytes, D3D10_BIND_VERTEX_BUFFER, false);
	}

	void Update(const T* b){
		T* data = nullptr;
		buffer->Map(D3D10_MAP_READ_WRITE, 0, &data);
		CopyMemory(data, b, TotalBytes);
		buffer->Unmap();
	}

	void Apply(){
		core->GetDevice()->IASetVertexBuffers(0, 1, &buffer, { Stride }, { 0 });
	}
};

template <int Length>
class D3DIndexBuffer : D3DBuffer {

public:
	enum {
		TotalBytes = sizeof(unsigned short)*Length
	};

	D3DIndexBuffer(){
		InitializeBuffer(nullptr, TotalBytes, D3D10_BIND_VERTEX_BUFFER, false);
	}
	D3DIndexBuffer(const unsigned short* data){
		InitializeBuffer(data, TotalBytes, D3D10_BIND_VERTEX_BUFFER, false);
	}

	void Update(const unsigned short* b){
		unsigned short* data = nullptr;
		buffer->Map(D3D10_MAP_READ_WRITE, 0, &data);
		CopyMemory(data, b, TotalBytes);
		buffer->Unmap();
	}

	void Apply(){
		auto device = core->GetDevice();
		device->IASetIndexBuffer(buffer, DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
	}
};

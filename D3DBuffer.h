#pragma once


class D3DBuffer;

#include "D3DCore.h"
#include "Resource.h"


class D3DBuffer :	public Resource {

protected:

	D3DCore* core;
	ID3D10Buffer *buffer;
	//std::weak_ptr<BufferData> mapped;

	D3DBuffer(D3DCore*);
public:
	virtual ~D3DBuffer();

	bool isDisposed();
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

template <typename T>
class D3DVertexBuffer : D3DBuffer {
	int length;
public:
	enum {
		Stride = sizeof(T),
	};

	D3DVertexBuffer(D3DCore *core, int length) : D3DBuffer(core) {
		this->length = length;
		InitializeBuffer(nullptr, Stride * length, D3D10_BIND_VERTEX_BUFFER, false);
	}

	D3DVertexBuffer(D3DCore *core, const T* data, int length, bool readonly = true) : D3DBuffer(core) {
		this->length = length;
		InitializeBuffer(data, Stride * length, D3D10_BIND_VERTEX_BUFFER, readonly);
	}

	template<int Length>
	D3DVertexBuffer(D3DCore* core, T(&data)[Length], bool readonly = true) : D3DBuffer(core) {
		this->length = Length;
		InitializeBuffer(data, Stride * length, D3D10_BIND_VERTEX_BUFFER, readonly);
	}


	void Update(const T* b){
		void* data = nullptr;
		buffer->Map(D3D10_MAP_READ_WRITE, 0, &data);
		CopyMemory(data, b, Stride * length);
		buffer->Unmap();
	}

	void Apply(){
		UINT stride[] = { Stride };
		UINT offset[] = { 0 };
		core->GetDevice()->IASetVertexBuffers(0, 1, &buffer, stride, offset);
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

#pragma once


class D3DBuffer;

#include "D3DCore.h"
#include "D3DShader.h"
#include "Resource.h"
#include "DirectXUtil.h"


class D3DBuffer :	public Resource {

protected:

	D3DCore* core;
	ID3D11Buffer *buffer;
	//std::weak_ptr<BufferData> mapped;

	D3DBuffer(D3DCore*);
public:
	virtual ~D3DBuffer();

	bool isDisposed();
	void Dispose();

	//std::shared_ptr<BufferData> GetData();

	ID3D11Buffer* GetRawBuffer() const;

protected:
	void InitializeBuffer(const void* bufferData, int bufferSize, UINT usage, bool readonly = true);
	void UpdateBuffer(const void* bufferData, int bufferSize);

};

//// ここから用途別に特殊化されたバッファ

template <typename T>
class D3DVertexBuffer : public D3DBuffer {
	int length;
public:
	enum {
		Stride = sizeof(T),
	};
	typedef T vertex_t;

	D3DVertexBuffer(D3DCore *core, int length) : D3DBuffer(core) {
		this->length = length;
		InitializeBuffer(nullptr, Stride * length, D3D11_BIND_VERTEX_BUFFER, false);
	}

	D3DVertexBuffer(D3DCore *core, const T* data, int length, bool readonly = true) : D3DBuffer(core) {
		this->length = length;
		InitializeBuffer(data, Stride * length, D3D11_BIND_VERTEX_BUFFER, readonly);
	}

	template<int Length>
	D3DVertexBuffer(D3DCore* core, T(&data)[Length], bool readonly = true) : D3DBuffer(core) {
		this->length = Length;
		InitializeBuffer(data, Stride * length, D3D11_BIND_VERTEX_BUFFER, readonly);
	}


	void Update(const T* b){
		UpdateBuffer(b, Stride * length);
	}

	void Apply(){
		UINT stride[] = { Stride };
		UINT offset[] = { 0 };
		core->GetDeviceContext()->IASetVertexBuffers(0, 1, &buffer, stride, offset);
	}

	void Unapply(){
		UINT stride[] = { Stride };
		UINT offset[] = { 0 };
		core->GetDeviceContext()->IASetVertexBuffers(0, 1, nullptr, stride, offset);
	}

	int GetLength() { return length; }
};

template <typename IndexT = unsigned short, int Format = DXGI_FORMAT::DXGI_FORMAT_R16_UINT>
class D3DIndexBuffer : public D3DBuffer {
	int length;
public:
	typedef IndexT index_t;

	D3DIndexBuffer(D3DCore *core, int length) : D3DBuffer(core) {
		this->length = length;
		InitializeBuffer(nullptr, sizeof(IndexT) * length, D3D11_BIND_INDEX_BUFFER, false);
	}

	D3DIndexBuffer(D3DCore *core, const IndexT* data, int length, bool readonly = true) : D3DBuffer(core) {
		this->length = length;
		InitializeBuffer(data, sizeof(IndexT)* length, D3D11_BIND_INDEX_BUFFER, readonly);
	}

	template<int Length>
	D3DIndexBuffer(D3DCore* core, IndexT(&data)[Length], bool readonly = true) : D3DBuffer(core) {
		this->length = Length;
		InitializeBuffer(data, sizeof(IndexT)* length, D3D11_BIND_INDEX_BUFFER, readonly);
	}


	void Update(const IndexT* b){
		UpdateBuffer(b, sizeof(IndexT)* length);
	}

	void Apply(){
		auto ctx = core->GetDeviceContext();
		ctx->IASetIndexBuffer(buffer, (DXGI_FORMAT)Format, 0);
	}

	void Unapply(){
		auto ctx = core->GetDeviceContext();
		ctx->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	}

	static void Unapply(D3DCore *core){
		auto ctx = core->GetDeviceContext();
		ctx->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	}


	int GetLength() { return length; }
};

template <typename T>
class D3DConstantBuffer : public D3DBuffer {
	friend class D3DEffect;
	//T data;

public:
	typedef T data_t;

	D3DConstantBuffer(D3DCore *core) : D3DBuffer(core) {
		InitializeBuffer(nullptr, sizeof(T), D3D11_BIND_CONSTANT_BUFFER, false);
	}
	D3DConstantBuffer(const D3DConstantBuffer<T>& deletedConstractor) = delete;


	void Update(const T* b){
		UpdateBuffer(b, sizeof(T));
	}

	void Apply(Shaders::ShaderFlag targetShader, int index) {
		auto ctx = core->GetDeviceContext();

		if (Shaders::CheckFlag(targetShader , Shaders::ShaderFlag::Vertex) ){
			ctx->VSSetConstantBuffers(index, 1, &this->buffer);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Pixel)){
			ctx->PSSetConstantBuffers(index, 1, &this->buffer);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Geometry)){
			ctx->GSSetConstantBuffers(index, 1, &this->buffer);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Compute)){
			ctx->PSSetConstantBuffers(index, 1, &this->buffer);
		}
	}

	void Unapply(Shaders::ShaderFlag targetShader, int index) {
		auto ctx = core->GetDeviceContext();

		ID3D11Buffer* nullbuffer[] = { nullptr };

		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Vertex)){
			ctx->VSSetConstantBuffers(index, 1, nullbuffer);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Pixel)){
			ctx->PSSetConstantBuffers(index, 1, nullbuffer);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Geometry)){
			ctx->GSSetConstantBuffers(index, 1, nullbuffer);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Compute)){
			ctx->PSSetConstantBuffers(index, 1, nullbuffer);
		}
	}

};

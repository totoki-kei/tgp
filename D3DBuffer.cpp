#include "D3DBuffer.h"
#include "DirectXUtil.h"

#pragma region BufferData
/*
D3DBuffer::BufferData::~BufferData(){
	Release();
}

void* D3DBuffer::BufferData::GetPointer() const {
	return data;
}

void D3DBuffer::BufferData::Release(){
	if (this->data){
		buffer->buffer->Unmap();
		this->data = nullptr;
	}
}
*/
#pragma endregion

D3DBuffer::D3DBuffer(D3DCore* core) : core(core) {
	buffer = nullptr;
//	mapped = std::weak_ptr<D3DBuffer::BufferData>();
}

D3DBuffer::~D3DBuffer(){
	if (buffer) buffer->Release();
}

void D3DBuffer::Dispose(){
	if (buffer) buffer->Release();
	Resource::Dispose();
}

void D3DBuffer::InitializeBuffer(void* bufferData, int bufferSize, UINT usage, bool readonly) {
	// すでに存在する場合は削除
	if (buffer) buffer->Release();

	D3D10_BUFFER_DESC desc;

	desc.BindFlags = usage;
	desc.ByteWidth = bufferSize;
	desc.CPUAccessFlags = (readonly ? 0 : (D3D10_CPU_ACCESS_WRITE));
	desc.MiscFlags = 0;
	desc.Usage = (readonly ? D3D10_USAGE_DEFAULT : D3D10_USAGE_DYNAMIC);

	HRESULT result;
	if (bufferData) {
		D3D10_SUBRESOURCE_DATA subres;
		subres.pSysMem = bufferData;
		subres.SysMemPitch = 0;
		subres.SysMemSlicePitch = 0;
		result = core->GetDevice()->CreateBuffer(&desc, &subres, &buffer);
	}
	else {
		result = core->GetDevice()->CreateBuffer(&desc, nullptr, &buffer);
	}
	IF_NG(result){
		DBG_OUT("failed to create buffer hresult = %X, args(%p, %d, %u, %d)", result, bufferData, bufferSize, usage, readonly);
	}
}

/*
std::shared_ptr<D3DBuffer::BufferData> D3DBuffer::GetData(){
	if (mapped.expired()) {
		auto buf = new BufferData();
		buffer->Map(D3D10_MAP_READ_WRITE, 0, &(buf->data));
		std::shared_ptr<D3DBuffer::BufferData> ret(buf);
		this->mapped = ret;
		return ret;
	}
	else{
		return std::shared_ptr<D3DBuffer::BufferData>();
	}
}
*/

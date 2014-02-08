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
	if (!isDisposed()) Dispose();
}

bool D3DBuffer::isDisposed(){
	return buffer != nullptr;
}

void D3DBuffer::Dispose(){
	buffer->Release();
	Resource::Dispose();
}

void D3DBuffer::InitializeBuffer(void* bufferData, int bufferSize, UINT usage, bool readonly) {
	// ‚·‚Å‚É‘¶Ý‚·‚éê‡‚Ííœ
	if (buffer) buffer->Release();

	D3D11_BUFFER_DESC desc;

	desc.BindFlags = usage;
	desc.ByteWidth = bufferSize;
	desc.CPUAccessFlags = (readonly ? 0 : (D3D11_CPU_ACCESS_WRITE));
	desc.MiscFlags = 0;
	desc.Usage = (readonly ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC);

	HRESULT result;
	if (bufferData) {
		D3D11_SUBRESOURCE_DATA subres;
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
		buffer->Map(D3D11_MAP_READ_WRITE, 0, &(buf->data));
		std::shared_ptr<D3DBuffer::BufferData> ret(buf);
		this->mapped = ret;
		return ret;
	}
	else{
		return std::shared_ptr<D3DBuffer::BufferData>();
	}
}
*/

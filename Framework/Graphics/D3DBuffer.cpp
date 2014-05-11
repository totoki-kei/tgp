#include "D3DBuffer.h"
#include "../DirectXUtil.h"

D3DBuffer::D3DBuffer(D3DCore* core) : core(core) {
	buffer = nullptr;
	//	mapped = std::weak_ptr<D3DBuffer::BufferData>();
}

D3DBuffer::~D3DBuffer(){
	if (!isDisposed()) Dispose();
}

bool D3DBuffer::isDisposed(){
	return buffer == nullptr;
}

void D3DBuffer::Dispose(){
	buffer->Release();
	buffer = nullptr;
	Resource::Dispose();
}

void D3DBuffer::InitializeBuffer(const void* bufferData, int bufferSize, UINT usage, bool readonly) {
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

void D3DBuffer::UpdateBuffer(const void* bufferData, int bufferSize){
	void* data = nullptr;

	D3D11_MAPPED_SUBRESOURCE msr;

	auto ctx = core->GetDeviceContext();
	auto hresult = ctx->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	IF_NG(hresult) {
		// ƒ}ƒbƒvŽ¸”s
		return;
	}
	data = msr.pData;
	CopyMemory(data, bufferData, bufferSize);
	//buffer->Unmap();
	ctx->Unmap(buffer, 0);

}


ID3D11Buffer* D3DBuffer::GetRawBuffer() const {
	return buffer;
}


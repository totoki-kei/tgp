#include "D3DBuffer.h"
#include "../DirectXUtil.h"

D3DBuffer::D3DBuffer(D3DCore* core) : core(core), mapped{ false } {
	buffer = nullptr;
}

D3DBuffer::~D3DBuffer(){
	if (!isDisposed()) Dispose();
}

bool D3DBuffer::isDisposed(){
	return buffer == nullptr;
}

void D3DBuffer::Dispose(){
	if (mapped) {
		unmapCallback();
	}
	buffer->Release();
	buffer = nullptr;
	Resource::Dispose();
}

void D3DBuffer::InitializeBuffer(const void* bufferData, size_t bufferSize, UINT usage, bool cpuAccessable) {
	// ‚·‚Å‚É‘¶Ý‚·‚éê‡‚Ííœ
	if (buffer) {
		if (mapped) {
			unmapCallback();
		}
		buffer->Release();
	}

	D3D11_BUFFER_DESC desc;

	desc.BindFlags = usage;
	desc.ByteWidth = (UINT)bufferSize;
	desc.CPUAccessFlags = (cpuAccessable ? (D3D11_CPU_ACCESS_WRITE) : 0);
	desc.MiscFlags = 0;
	desc.Usage = (cpuAccessable ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT);

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
		LOG_DBG("failed to create buffer hresult = %X, args(%p, %d, %u, %d)", result, bufferData, bufferSize, usage, cpuAccessable);
	}

	this->canCpuAccess = cpuAccessable;
}

void D3DBuffer::UpdateBuffer(const void* bufferData, size_t bufferSize) {
	if (canCpuAccess) {
		UpdateBuffer(bufferData, 0, bufferSize);
	}
	else {
		auto ctx = core->GetDeviceContext();
		ctx->UpdateSubresource(buffer, 0, nullptr, bufferData, 0, 0);
	}
}

void D3DBuffer::UpdateBuffer(const void* bufferData, size_t offset, size_t bufferSize) {
	BYTE* data = nullptr;

	if (canCpuAccess) {
		D3D11_MAPPED_SUBRESOURCE msr;

		auto ctx = core->GetDeviceContext();
		auto hresult = ctx->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		IF_NG(hresult) {
			// ƒ}ƒbƒvŽ¸”s
			return;
		}
		data = ((BYTE*)msr.pData) + offset;
		CopyMemory(data, bufferData, bufferSize);
		//buffer->Unmap();
		ctx->Unmap(buffer, 0);
	}
	else {
		auto ctx = core->GetDeviceContext();
		D3D11_BOX region = { 0U };
		region.left = (UINT)offset;
		region.right = (UINT)(offset + bufferSize);
		region.top = 0;
		region.bottom = 1;
		region.front = 0;
		region.back = 1;
		ctx->UpdateSubresource(buffer, 0, &region, bufferData, 0, 0);
	}
}


ID3D11Buffer* D3DBuffer::GetRawBuffer() const {
	return buffer;
}

D3DCore* D3DBuffer::GetCore() const {
	return core;
}


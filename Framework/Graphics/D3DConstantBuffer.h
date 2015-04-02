#pragma once
#include "D3DBuffer.h"

template <typename T>
class D3DConstantBuffer : public D3DBuffer {
	friend class D3DEffect;
	//T data;

public:
	typedef T data_t;

	D3DConstantBuffer(D3DCore *core, bool cpuAccessable = true) : D3DBuffer(core) {
		InitializeBuffer(nullptr, sizeof(T), D3D11_BIND_CONSTANT_BUFFER, cpuAccessable);
	}
	D3DConstantBuffer(const D3DConstantBuffer<T>& deletedConstractor) = delete;


	void Update(const T* b) {
		UpdateBuffer(b, sizeof(T));
	}

	void Apply(Shaders::ShaderFlag targetShader, int index) {
		auto ctx = core->GetDeviceContext();

		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Vertex)) {
			ctx->VSSetConstantBuffers(index, 1, &this->buffer);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Pixel)) {
			ctx->PSSetConstantBuffers(index, 1, &this->buffer);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Geometry)) {
			ctx->GSSetConstantBuffers(index, 1, &this->buffer);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Compute)) {
			ctx->CSSetConstantBuffers(index, 1, &this->buffer);
		}
	}

	static void Unapply(D3DCore *core, Shaders::ShaderFlag targetShader, int index) {
		auto ctx = core->GetDeviceContext();

		ID3D11Buffer* nullbuffer[] = { nullptr };

		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Vertex)) {
			ctx->VSSetConstantBuffers(index, 1, nullbuffer);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Pixel)) {
			ctx->PSSetConstantBuffers(index, 1, nullbuffer);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Geometry)) {
			ctx->GSSetConstantBuffers(index, 1, nullbuffer);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Compute)) {
			ctx->PSSetConstantBuffers(index, 1, nullbuffer);
		}
	}

};


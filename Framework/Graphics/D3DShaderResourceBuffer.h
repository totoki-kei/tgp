#pragma once
#include "D3DBuffer.h"

template <typename T, size_t Size, int Format = DXGI_FORMAT_R32G32B32A32_FLOAT>
class D3DShaderResourceBuffer : public D3DBuffer {
	friend class D3DEffect;
	//T data;

	ID3D11ShaderResourceView* view;

public:
	typedef T data_t;

	D3DShaderResourceBuffer(D3DCore *core, bool cpuAccessable = true) : D3DBuffer(core) {
		InitializeBuffer(nullptr, sizeof(T) * Size, D3D11_BIND_SHADER_RESOURCE, cpuAccessable);
		CD3D11_SHADER_RESOURCE_VIEW_DESC desc(buffer, (DXGI_FORMAT)Format, 0, (sizeof(T) / FormatSize<Format>::bytes) * Size);
		HRESULT result = core->GetDevice()->CreateShaderResourceView(this->buffer, &desc, &view);
		IF_NG(result) {
			LOG_DBG("failed to create ShaderResourceView hresult = %X\n", result);
		}
		NameToResourceFormated(view, "D3DShaderResourceBuffer::ShaderResourceView<>()");
		this->AddResource(HndToRes(view));
	}
	D3DShaderResourceBuffer(const D3DShaderResourceBuffer<T, Size>&) = delete;
	D3DShaderResourceBuffer() = delete;

	void Update(const T* b) {
		UpdateBuffer(b, sizeof(T) * Size);
	}

	void Update(const T* b, size_t count, size_t offset) {
		UpdateBuffer(b, offset * sizeof(T), count * sizeof(T));
	}

	void Apply(Shaders::ShaderFlag targetShader, int index) {
		auto ctx = core->GetDeviceContext();

		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Vertex)) {
			ctx->VSSetShaderResources(index, 1, &this->view);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Pixel)) {
			ctx->PSSetShaderResources(index, 1, &this->view);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Geometry)) {
			ctx->GSSetShaderResources(index, 1, &this->view);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Compute)) {
			ctx->CSSetShaderResources(index, 1, &this->view);
		}
	}

	static void Unapply(D3DCore *core, Shaders::ShaderFlag targetShader, int index) {
		auto ctx = core->GetDeviceContext();

		ID3D11Buffer* nullbuffer[] = { nullptr };

		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Vertex)) {
			ctx->VSSetShaderResources(index, 1, nullbuffer);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Pixel)) {
			ctx->PSSetShaderResources(index, 1, nullbuffer);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Geometry)) {
			ctx->GSSetShaderResources(index, 1, nullbuffer);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Compute)) {
			ctx->PSSetShaderResources(index, 1, nullbuffer);
		}
	}

};

#include "D3DShader.h"

#include "DirectXUtil.h"

namespace Shaders {

	VertexShader::VertexShader(D3DCore* core, const BYTE* data, SIZE_T dataSize){
		this->core = core;
		auto device = core->GetDevice();
		auto result = device->CreateVertexShader(data, dataSize, nullptr, &this->shader);
		IF_NG(result){
			DBG_OUT("Failed to create shader.");
			this->shader = nullptr;
			return;
		}
		this->bytecode = data;
		this->bytecodeSize = dataSize;
	}

	VertexShader::~VertexShader(){
		if (!isDisposed()) Dispose();
	}

	bool VertexShader::isDisposed(){
		return shader == nullptr;
	}

	void VertexShader::Dispose(){
		shader->Release();
		shader = nullptr;
		Resource::Dispose();
	}

	void VertexShader::Apply(){
		auto context = core->GetDeviceContext();
		context->VSSetShader(this->shader, nullptr, 0);
	}




	PixelShader::PixelShader(D3DCore* core, const BYTE* data, SIZE_T dataSize){
		this->core = core;
		auto device = core->GetDevice();
		auto result = device->CreatePixelShader(data, dataSize, nullptr, &this->shader);
		IF_NG(result){
			DBG_OUT("Failed to create shader.");
			this->shader = nullptr;
			return;
		}
		this->bytecode = data;
		this->bytecodeSize = dataSize;
	}

	PixelShader::~PixelShader(){
		if (!isDisposed()) Dispose();
	}

	bool PixelShader::isDisposed(){
		return shader == nullptr;
	}

	void PixelShader::Dispose(){
		shader->Release();
		shader = nullptr;
		Resource::Dispose();
	}

	void PixelShader::Apply(){
		auto context = core->GetDeviceContext();
		context->PSSetShader(this->shader, nullptr, 0);
	}




	GeometryShader::GeometryShader(D3DCore* core, const BYTE* data, SIZE_T dataSize){
		this->core = core;
		auto device = core->GetDevice();
		auto result = device->CreateGeometryShader(data, dataSize, nullptr, &this->shader);
		IF_NG(result){
			DBG_OUT("Failed to create shader.");
			this->shader = nullptr;
			return;
		}
		this->bytecode = data;
		this->bytecodeSize = dataSize;
	}

	GeometryShader::~GeometryShader(){
		if (!isDisposed()) Dispose();
	}

	bool GeometryShader::isDisposed(){
		return shader == nullptr;
	}

	void GeometryShader::Dispose(){
		shader->Release();
		shader = nullptr;
		Resource::Dispose();
	}

	void GeometryShader::Apply(){
		auto context = core->GetDeviceContext();
		context->GSSetShader(this->shader, nullptr, 0);
	}




	ComputeShader::ComputeShader(D3DCore* core, const BYTE* data, SIZE_T dataSize){
		this->core = core;
		auto device = core->GetDevice();
		auto result = device->CreateComputeShader(data, dataSize, nullptr, &this->shader);
		IF_NG(result){
			DBG_OUT("Failed to create shader.");
			this->shader = nullptr;
			return;
		}
		this->bytecode = data;
		this->bytecodeSize = dataSize;
	}


	ComputeShader::~ComputeShader(){
		if (!isDisposed()) Dispose();
	}

	bool ComputeShader::isDisposed(){
		return shader == nullptr;
	}

	void ComputeShader::Dispose(){
		shader->Release();
		shader = nullptr;
		Resource::Dispose();
	}

	void ComputeShader::Apply(){
		auto context = core->GetDeviceContext();
		context->CSSetShader(this->shader, nullptr, 0);
	}


	void Unapply(D3DCore* core, ShaderFlag targetShader){
		auto ctx = core->GetDeviceContext();

		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Vertex)){
			ctx->VSSetShader(nullptr, nullptr, 0);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Pixel)){
			ctx->PSSetShader(nullptr, nullptr, 0);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Geometry)){
			ctx->GSSetShader(nullptr, nullptr, 0);
		}
		if (Shaders::CheckFlag(targetShader, Shaders::ShaderFlag::Compute)){
			ctx->PSSetShader(nullptr, nullptr, 0);
		}

	}

}


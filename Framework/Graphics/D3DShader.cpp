#include <initializer_list>

#include "D3DShader.h"

#include "../DirectXUtil.h"

namespace Shaders {

	VertexShader::VertexShader(D3DCore* core, const BYTE* data, SIZE_T dataSize, ClassLinkage* linkage){
		this->core = core;
		auto device = core->GetDevice();
		auto result = device->CreateVertexShader(data, dataSize, linkage ? linkage->$Linkage : nullptr, &this->shader);
		IF_NG(result){
			LOG_ERR("Failed to create shader.");
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

	void VertexShader::Apply(const ClassInstanceList& clsinst){
		auto context = core->GetDeviceContext();
		context->VSSetShader(this->shader, clsinst.PtrArray, (UINT)clsinst.Count);
	}


	PixelShader::PixelShader(D3DCore* core, const BYTE* data, SIZE_T dataSize, ClassLinkage* linkage){
		this->core = core;
		auto device = core->GetDevice();
		auto result = device->CreatePixelShader(data, dataSize, linkage ? linkage->$Linkage : nullptr, &this->shader);
		IF_NG(result){
			LOG_ERR("Failed to create shader.");
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

	void PixelShader::Apply(const ClassInstanceList& clsinst){
		auto context = core->GetDeviceContext();
		context->PSSetShader(this->shader, clsinst.PtrArray, (UINT)clsinst.Count);
	}




	GeometryShader::GeometryShader(D3DCore* core, const BYTE* data, SIZE_T dataSize, ClassLinkage* linkage){
		this->core = core;
		auto device = core->GetDevice();
		auto result = device->CreateGeometryShader(data, dataSize, linkage ? linkage->$Linkage : nullptr, &this->shader);
		IF_NG(result){
			LOG_ERR("Failed to create shader.");
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

	void GeometryShader::Apply(const ClassInstanceList& clsinst){
		auto context = core->GetDeviceContext();
		context->GSSetShader(this->shader, clsinst.PtrArray, (UINT)clsinst.Count);
	}



	ComputeShader::ComputeShader(D3DCore* core, const BYTE* data, SIZE_T dataSize, ClassLinkage* linkage){
		this->core = core;
		auto device = core->GetDevice();
		auto result = device->CreateComputeShader(data, dataSize, linkage ? linkage->$Linkage : nullptr, &this->shader);
		IF_NG(result){
			LOG_ERR("Failed to create shader.");
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


	void ComputeShader::Apply(const ClassInstanceList& clsinst){
		auto context = core->GetDeviceContext();
		context->CSSetShader(this->shader, clsinst.PtrArray, (UINT)clsinst.Count);
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



	ClassLinkage::ClassLinkage(D3DCore* core){
		auto device = core->GetDevice();
		device->CreateClassLinkage(&this->linkage);
		NameToResource(linkage, "ClassLinkage");
		core->AddResource(HndToRes(linkage));
	}

	ClassInstance* ClassLinkage::Create(const char* className){
		ID3D11ClassInstance *instance;
		linkage->CreateClassInstance(
			className, 0, 0, 0, 0, &instance
			);
		// CreateClassInstanceの引数に定数バッファオフセットなどを設定するバージョンは
		// メンバフィールドを持つclassをcreateする必要が出てから作る

		NameToResourceFormated(instance, "ClassInstance_new_%s", className);
		auto ret = new ClassInstance(instance);
		this->AddResource(PtrToRes(ret));
		return ret;
	}

	ClassInstance* ClassLinkage::Get(const char* instanceName, int instanceIndex){
		ID3D11ClassInstance *instance;
		linkage->GetClassInstance(instanceName, instanceIndex, &instance);

		NameToResourceFormated(instance, "ClassInstance_%s", instanceIndex);
		auto ret = new ClassInstance(instance);
		this->AddResource(PtrToRes(ret));
		return ret;


		D3D11_CLASS_INSTANCE_DESC desc;
		instance->GetDesc(&desc);
	}

	ClassInstance::ClassInstance(ID3D11ClassInstance* instance){
		this->inst = instance;
		AddResource(HndToRes(instance));
	}

}


#include "D3DEffect.h"

#include "DirectXUtil.h"

#include "Debug.h"


D3DEffect::D3DEffect(D3DCore * core, const TCHAR* effectFile) : 
	core(core),
	name(effectFile)
{
	auto device = core->GetDevice();

	auto ret = D3DX10CreateEffectFromFile(
		name.c_str(), 
		NULL,
		NULL,
		"fx_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		device,
		NULL,
		NULL,
		&this->effect,
		NULL,
		NULL);

	IF_NG(ret) {
		effect = nullptr;
		return ;
	}

	this->loadtype = LoadType::File;

	this->AddResource(HndToRes(effect));
}

D3DEffect::D3DEffect(D3DCore * core, const TCHAR* effectName, const BYTE data[], int dataLength) : 
	core(core),
	name(effectName)
{
	auto device = core->GetDevice();

	auto ret = D3DX10CreateEffectFromMemory(
		data,
		dataLength,
		name.c_str(),
		NULL,
		NULL,
		"fx_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		device,
		NULL,
		NULL,
		&this->effect,
		&this->blob,
		NULL);

	IF_NG(ret) {
		effect = nullptr;
		return ;
	}

	this->loadtype = LoadType::Memory;

	this->AddResource(HndToRes(effect));
}

D3DEffect::~D3DEffect(void)
{
	Dispose();
}

bool D3DEffect::isDisposed() { 
	return effect == nullptr;
}

void D3DEffect::Dispose() {
	if(isDisposed()) return;
	Resource::Dispose();
//	effect->Release(); // effectはDisposeでリソースとして開放されるためReleaseは不要
	effect = nullptr;
}

#pragma region Technique


D3DEffect::Technique::Technique(D3DEffect &e, int i) : parent(e) {
	tech = e.effect->GetTechniqueByIndex(i);
	D3D10_TECHNIQUE_DESC td;
	tech->GetDesc(&td);
	passCount = td.Passes;
	SetPass(0);
}

D3DEffect::Technique::Technique(D3DEffect &e, const TCHAR* n) : parent(e) {
	tech = e.effect->GetTechniqueByName(n);
	D3D10_TECHNIQUE_DESC td;
	tech->GetDesc(&td);
	passCount = td.Passes;
	SetPass(0);
}

bool D3DEffect::Technique::IsValidTechnique() {
	return tech != nullptr;
}

void D3DEffect::Technique::SetPass(int i){
	if(i < 0 || passCount <= i) {
		// エラー通知？
		return;
	}

	pass = tech->GetPassByIndex(i);
	passIndex = i;
}

int D3DEffect::Technique::GetPass() {
	return passIndex;
}

int D3DEffect::Technique::GetPassCount() {
	return passCount;
}

void D3DEffect::Technique::MoveNext() {
	SetPass(++passIndex);
}

bool D3DEffect::Technique::EndOfPass() {
	return (passIndex < 0 || passCount <= passIndex);
}

void D3DEffect::Technique::ApplyPass() {
	if(pass == nullptr) return;
	HRESULT result;
	IF_NG2(pass->Apply(0), result){
		// エラー処理？
		DBG_OUT("applying pass failed ret=%d, tech=%p, pass=%p",result, tech, pass);
	}
}

D3DEffect* D3DEffect::Technique::GetEffect() const {
	return &this->parent;
}

#pragma endregion

#pragma region ConstantBufferBase

D3DEffect::ConstantBufferBase::ConstantBufferBase(D3DEffect& e)
: parent(e){

}

D3DEffect::ConstantBufferBase::~ConstantBufferBase(){

}

void D3DEffect::ConstantBufferBase::CreateBuffer(int dataSize, ID3D10EffectConstantBuffer* cb, int idx) {
	D3D10_BUFFER_DESC bd = {0};
	cbuffer = cb;
	cbuffer->GetDesc(&valdesc);

	bd.MiscFlags = D3D10_BIND_CONSTANT_BUFFER;
	bd.ByteWidth = dataSize;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;  
	bd.MiscFlags = 0;  
	bd.Usage = D3D10_USAGE_DYNAMIC;  
	parent.GetCore()->GetDevice()->CreateBuffer(
		&bd,
		NULL,
		&buf);

	cbuffer->SetConstantBuffer(buf);
	
	parent.AddResource(HndToRes(buf));
}


#pragma endregion

D3DCore* D3DEffect::GetCore(){
	return this->core;
}

D3DEffect::Technique D3DEffect::GetTechnique(int techIndex){
	return D3DEffect::Technique(*this, techIndex);
}

D3DEffect::Technique D3DEffect::GetTechnique(const TCHAR* techName){
	return D3DEffect::Technique(*this, techName);
}

void* D3DEffect::GetBytecode(){
	return blob->GetBufferPointer();
}


/*
void D3DEffect::SetConstantBuffer(const TCHAR* name, void* data, int dataSize){
	auto wcb = FindConstantBuffer(name);
	auto cb = wcb.lock();
	if (!cb) {
		DBG_OUT("");
	}
	cb->cbuffer->GetRawValue(data, 0, dataSize);
}
void D3DEffect::GetConstantBuffer(const TCHAR* name, void* data, int dataSize){


}
*/


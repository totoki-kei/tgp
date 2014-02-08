#include "D3DEffect.h"

#include "DirectXUtil.h"

#include "Debug.h"

#include <d3dcompiler.h>

D3DEffect::D3DEffect(D3DCore * core, const TCHAR* effectFile) : 
	core(core),
	name(effectFile)
{

	LoadFileToMemory(effectFile);
	this->loadtype = LoadType::File;

}

void D3DEffect::LoadFileToMemory(const TCHAR* filename){
	auto hnd = CreateFile(filename, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, 0);
	if (hnd == INVALID_HANDLE_VALUE) {
		srcData = nullptr;
		srcSize = -1;
		return;
	}

	int sizeL, sizeH = 0;
	sizeL = GetFileSize(hnd, &sizeH);
	if (sizeH > 0){
		// ファイル大きすぎてダメ
		srcData = nullptr;
		srcSize = -1;
		return;
	}

	this->srcSize = sizeL;

	srcData = new BYTE[srcSize];

	int readBytes = 0;
	ReadFile(hnd, srcData, srcSize, &readBytes, nullptr);

	CloseHandle(hnd);
}

D3DEffect::D3DEffect(D3DCore * core, const TCHAR* effectName, const BYTE data[], int dataLength) : 
	core(core),
	name(effectName)
{
	this->srcData = new BYTE[dataLength];
	CopyMemory(this->srcData, data, dataLength);
	this->srcSize = dataLength;

	this->loadtype = LoadType::Memory;

}

D3DEffect::~D3DEffect(void)
{
	if (!isDisposed()) Dispose();
}

bool D3DEffect::isDisposed() { 
	return srcData == nullptr;
}

void D3DEffect::Dispose() {
	if(isDisposed()) return;
	Resource::Dispose();
//	effect->Release(); // effectはDisposeでリソースとして開放されるためReleaseは不要
	delete[] srcData;
}

#pragma region Technique

D3DEffect::Technique::Technique(){
	tech = nullptr;
}

D3DEffect::Technique::Technique(D3DEffect* e, int i) : effect(e) {
	tech = e->effect->GetTechniqueByIndex(i);
	D3D11_TECHNIQUE_DESC td;
	tech->GetDesc(&td);
	passCount = td.Passes;
	SetPass(0);
}

D3DEffect::Technique::Technique(D3DEffect* e, const TCHAR* n) : effect(e) {
	tech = e->effect->GetTechniqueByName(n);
	D3D11_TECHNIQUE_DESC td;
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
	return this->effect;
}

#pragma endregion

#pragma region ConstantBufferBase

D3DEffect::ConstantBufferBase::ConstantBufferBase(D3DEffect& e)
: parent(e){

}

D3DEffect::ConstantBufferBase::~ConstantBufferBase(){

}

void D3DEffect::ConstantBufferBase::CreateBuffer(int dataSize) {
	HRESULT hr;

	D3D11_BUFFER_DESC bd = {0};
	//cbuffer = cb;
	//cbuffer->GetDesc(&valdesc);

	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.ByteWidth = dataSize;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;  
	bd.Usage = D3D11_USAGE_DYNAMIC;
	
	IF_NG2(parent.GetCore()->GetDevice()->CreateBuffer(&bd, NULL, &buf), hr) {
		// 作成に失敗している
		DBG_OUT("creating constant buffer failed ret=%d(%08X)", hr, hr);
	}
	
	parent.AddResource(HndToRes(buf));
}


#pragma endregion

D3DCore* D3DEffect::GetCore(){
	return this->core;
}

D3DEffect::Technique D3DEffect::GetTechnique(int techIndex){
	return D3DEffect::Technique(this, techIndex);
}

D3DEffect::Technique D3DEffect::GetTechnique(const TCHAR* techName){
	return D3DEffect::Technique(this, techName);
}

BYTE* D3DEffect::Technique::GetPassInputSignature(int index, int* outLength) {
	D3D10_PASS_DESC desc;

	auto path = tech->GetPassByIndex(index);
	if (pass == nullptr) {
		*outLength = 0;
		return nullptr;
	}
	path->GetDesc(&desc);
	*outLength = desc.IAInputSignatureSize;
	return desc.pIAInputSignature;

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


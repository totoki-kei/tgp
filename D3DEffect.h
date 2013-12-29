#pragma once

#include "Resource.h"
#include "D3DCore.h"

#include <D3D10effect.h>

class D3DEffect : public Resource
{
public:
	class Technique {
		friend class D3DEffect;
		
		D3DEffect &parent;
		ID3D10EffectTechnique* tech;
		ID3D10EffectPass* pass;

		int passCount;
		int passIndex;

		Technique(D3DEffect& e, int i);
		Technique(D3DEffect& e, const TCHAR* n);

	public:
		D3DEffect* GetEffect() const;
		D3DCore* GetCore() const;

		bool IsValidTechnique();
		void SetPass(int);
		int GetPass();
		int GetPassCount();
		void MoveNext();
		bool EndOfPass();

		void ApplyPass();
	};

	// 定数バッファ
	// リソース扱いはしない（上のEffectでまとめて管理する）
	class ConstantBufferBase {
		friend class D3DEffect;
		D3DEffect &parent;
		ID3D10EffectConstantBuffer *cbuffer;
		ID3D10Buffer *buf;
		D3D10_EFFECT_VARIABLE_DESC valdesc;

		std::shared_ptr<ConstantBufferBase> next;

	protected:
		ConstantBufferBase(D3DEffect&) ;
		virtual ~ConstantBufferBase() ;

		void CreateBuffer(int dataSize, ID3D10EffectConstantBuffer* cb, int idx);
	};

	template <typename T>
	class ConstantBuffer : ConstantBufferBase {
		T data;

		ConstantBuffer(D3DEffect& e, int i){
			CreateBuffer(sizeof(T), cb, i);
		}
		ConstantBuffer(D3DEffect& e, const TCHAR* n){
			CreateBuffer(sizeof(T), effect->GetConstantBufferByIndex(i), n);

		}

	public:

		T& GetValue() const { // もしかしたらconstやめるかもしれない
			return data;
		}
		void Update() {
			cbuffer->SetRawValue(&data, 0, sizeof(T));
		}
		void Reload() {
			cbuffer->GetRawValue(&data, 0, sizeof(T));
		}

	};

protected:
	D3DCore * core;

	ID3D10Effect* effect;

	enum class LoadType {
		File,
		Memory,
		Resource,
	} loadtype;

	tstring name;

	std::shared_ptr<ConstantBufferBase> cbufferList;


public: // methods
	D3DEffect(D3DCore *, const TCHAR*);
	D3DEffect(D3DCore *, const TCHAR*, const BYTE[], int);

	template<int ISize>
	D3DEffect(D3DCore *c, const TCHAR *n, const BYTE d[ISize])
		: D3DEffect(c, n, d, ILength) {}

	~D3DEffect(void);

	bool isDisposed();
	void Dispose();

	D3DCore* GetCore();

	Technique GetTechnique(int);
	Technique GetTechnique(const TCHAR*);

	// ConstantBuffer<T>経由で操作してもらうため、この辺りは不要になる
	//void SetConstantBuffer(const TCHAR* name, void* data, int dataSize);
	//void GetConstantBuffer(const TCHAR* name, void* data, int dataSize);

	//template <typename TBuffer>
	//void SetConstantBuffer(const TCHAR* name, TBuffer& val){
	//	SetConstantBuffer(name, &val, sizeof(TBuffer));
	//}

	//template <typename TBuffer>
	//void GetConstantBuffer(const TCHAR* name, TBuffer& val){
	//	GetConstantBuffer(name, &val, sizeof(TBuffer));
	//}


	template <typename T>
	std::weak_ptr<D3DEffect::ConstantBuffer<T> > GetConstantBuffer(const TCHAR* name){
		typedef D3DEffect::ConstantBuffer<T> BufferT;

		for (auto pp = cbufferList;
			pp != nullptr;
			pp = pp->next){

			if (_stricmp(pp->valdesc.Name, name) == 0){
				return dynamic_pointer_cast<BufferT>(pp);
			}
		}
		// 見つからなかったので空の要素を返す
		return std::weak_ptr<BufferT>();
	}

	template<typename T>
	void GetVariable(const TCHAR* name, T* data){
		auto v = effect->GetVariableByName(name);
		v->GetRawValue(data, 0, sizeof(T));
	}
	template<typename T>
	void SetVariable(const TCHAR* name, T* data){
		auto v = effect->GetVariableByName(name);
		v->SetRawValue(data, 0, sizeof(T));
	}


protected: // inner methods


};

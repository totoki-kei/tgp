#pragma once


class D3DBuffer;

#include "D3DCore.h"
#include "D3DShader.h"
#include "../Resource.h"
#include "../DirectXUtil.h"

#include <functional>

class D3DBuffer :	public Resource {

protected:

	D3DCore* core;
	ID3D11Buffer *buffer;
	bool canCpuAccess;
	bool mapped;
	std::function<void(void)> unmapCallback;

	D3DBuffer(D3DCore*);
public:
	virtual ~D3DBuffer();

	bool isDisposed();
	void Dispose();

	ID3D11Buffer* GetRawBuffer() const;
	D3DCore* GetCore() const;

	template <typename T>
	class Mapper {
		typedef Mapper<T> MapperType;

		friend class D3DBuffer;
		D3DBuffer* buffer;
		T* ptr;

	public:

		// バッファのMapを呼び、ポインタを自身に格納する
		// すでに何らかのバッファをマップ済みであった場合はUnmapされる
		void Map(D3DBuffer* b) {
			if (!b->canCpuAccess) return;
			Unmap();

			if (b->mapped) {
				// 二重にマップは無理
				return;
			}

			b->mapped = true;
			b->unmapCallback = [this]() { this->Unmap(); };

			D3D11_MAPPED_SUBRESOURCE msr;

			auto ctx = b->GetCore()->GetDeviceContext();
			auto hresult = ctx->Map(
				b->buffer,
				0,
				D3D11_MAP_WRITE_DISCARD,
				0,
				&msr);

			IF_NG(hresult) {
				// マップ失敗
				b->mapped = false;
				return;
			}

			this->buffer = b;
			this->ptr = reinterpret_cast<T*>(msr.pData);
		}

		// マッピングを解除し、バッファへの参照を外す。
		void Unmap() {
			if (buffer && ptr) {
				auto ctx = buffer->GetCore()->GetDeviceContext();
				ctx->Unmap(buffer->GetRawBuffer(), 0);
				buffer->mapped = false;
				buffer = nullptr;
				ptr = nullptr;
			}
		}

		// デフォルトコンストラクタ：無効なインスタンスを生成
		Mapper()
			: buffer{ nullptr }
			, ptr{ nullptr } {}

		// コピーコンストラクタ：使用不可
		Mapper(const MapperType&) = delete;

		// ムーブコンストラクタ：参照内容を移動
		Mapper(MapperType&& m)
			: buffer{ m.buffer }
			, ptr{ m.ptr } 
		{
			m.buffer = nullptr;
			m.ptr = nullptr;
			buffer->unmapCallback = [this]() { this->Unmap(); };
		}

		// バッファからインスタンスを構成するコンストラクタ
		Mapper(D3DBuffer* b)
			: buffer{ nullptr }
			, ptr{ nullptr } 
		{
			Map(b);
		}

		MapperType& operator = (MapperType&& m) {
			Unmap();
			this->buffer = m.buffer;
			this->ptr = m.ptr;
			m.buffer = nullptr;
			m.ptr = nullptr;
		}
		
		~Mapper() {
			Unmap();
		}

		D3DBuffer* GetBuffer() const { return buffer; }
		T* GetPtr() const { return ptr; }

		__declspec(property(get = GetBuffer))
			D3DBuffer* $Buffer;
		__declspec(property(get = GetPtr))
			T* $Pointer;


		const T& operator *() const { return *ptr; }
		T& operator *() { return *ptr; }
		const T* operator ->() const { return ptr; }
		T* operator ->() { return ptr; }
		operator bool() const { return buffer && ptr; }

		template <typename IndexT>
		T& operator [](IndexT idx) { return ptr[idx]; }
		template <typename IndexT>
		const T& operator [](IndexT idx) const { return ptr[idx]; }
	};

	template <typename T = void>
	Mapper<T> Map() {
		return Mapper<T>{ this };
	}

protected:
	void InitializeBuffer(const void* bufferData, size_t bufferSize, UINT usage, bool cpuAccessable = false);
	void UpdateBuffer(const void* bufferData, size_t bufferSize);
	void UpdateBuffer(const void* bufferData, size_t offset, size_t bufferSize);
public:


};


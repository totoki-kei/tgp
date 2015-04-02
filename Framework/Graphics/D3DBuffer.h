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

		// �o�b�t�@��Map���ĂсA�|�C���^�����g�Ɋi�[����
		// ���łɉ��炩�̃o�b�t�@���}�b�v�ς݂ł������ꍇ��Unmap�����
		void Map(D3DBuffer* b) {
			if (!b->canCpuAccess) return;
			Unmap();

			if (b->mapped) {
				// ��d�Ƀ}�b�v�͖���
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
				// �}�b�v���s
				b->mapped = false;
				return;
			}

			this->buffer = b;
			this->ptr = reinterpret_cast<T*>(msr.pData);
		}

		// �}�b�s���O���������A�o�b�t�@�ւ̎Q�Ƃ��O���B
		void Unmap() {
			if (buffer && ptr) {
				auto ctx = buffer->GetCore()->GetDeviceContext();
				ctx->Unmap(buffer->GetRawBuffer(), 0);
				buffer->mapped = false;
				buffer = nullptr;
				ptr = nullptr;
			}
		}

		// �f�t�H���g�R���X�g���N�^�F�����ȃC���X�^���X�𐶐�
		Mapper()
			: buffer{ nullptr }
			, ptr{ nullptr } {}

		// �R�s�[�R���X�g���N�^�F�g�p�s��
		Mapper(const MapperType&) = delete;

		// ���[�u�R���X�g���N�^�F�Q�Ɠ��e���ړ�
		Mapper(MapperType&& m)
			: buffer{ m.buffer }
			, ptr{ m.ptr } 
		{
			m.buffer = nullptr;
			m.ptr = nullptr;
			buffer->unmapCallback = [this]() { this->Unmap(); };
		}

		// �o�b�t�@����C���X�^���X���\������R���X�g���N�^
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


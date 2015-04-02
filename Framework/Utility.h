#pragma once

#include "WinAPI.h"

#include <functional>
#include <DirectXMath.h>

size_t ConvertStringWidth(char *dst, size_t dstSize, const char *src);
size_t ConvertStringWidth(char *dst, size_t dstSize, const wchar_t *src);
size_t ConvertStringWidth(wchar_t *dst, size_t dstSize, const char *src);
size_t ConvertStringWidth(wchar_t *dst, size_t dstSize, const wchar_t *src);

size_t GetMultibyteStirngLength(const char* txt);
size_t GetMultibyteStirngLength(const wchar_t* txt);
size_t GetWideCharStringLength(const char* txt);
size_t GetWideCharStringLength(const wchar_t* txt);


BYTE* LoadFileToMemory(const TCHAR* filename, /* out */ int *size);

struct ScopeExit {
	std::function<void(void)> callback;

	template <typename Fn>
	ScopeExit& operator [](Fn& f) {
		if (callback) {
			auto oldcb = std::move(callback);
			callback = [oldcb, f]() {f(); oldcb(); };
		}
		else {
			callback = f;
		}
		return *this;
	}

	~ScopeExit() {
		if (callback) callback();
	}
};

template <typename Deriv>
struct HeapAligned {
	
	static void* operator new(size_t size){
		return _aligned_malloc(size, std::alignment_of<Deriv>::value);
	}

	static void* operator new[](size_t size) {
		return _aligned_malloc(size, std::alignment_of<Deriv>::value);
	}

	static void operator delete(void* pv) {
		_aligned_free(pv);
	}

	static void operator delete[](void* pv) {
		_aligned_free(pv);
	}
};

struct Initializer {
	template <typename Fn>
	Initializer(Fn f) {
		f();
	}
};

float CalcDistanceLineToPoint(const DirectX::XMVECTOR& a, const DirectX::XMVECTOR& b, const DirectX::XMVECTOR& p);


template <typename T>
class new_allocator {
	// Allocates block of memory:
	// - (opt) padding
	// - offset: ptrdiff_t
	// - T * n: T
	// - (opt) padding
public:
	typedef T* pointer;
	typedef size_t size_type;
	typedef T value_type;

	new_allocator() = default;
	new_allocator(const new_allocator<T>&) = default;

	template <class U>
	new_allocator(const new_allocator<U>&) {}


	pointer allocate(size_type n);
	void deallocate(pointer p, size_type n);

}; // class new_allocator

template <typename T>
auto new_allocator<T>::allocate(size_type n) -> pointer {
	return new T[n];
} // new_allocator<T>::allocate

template <typename T>
void new_allocator<T>::deallocate(pointer p, size_type) {
	delete[] p;
} // new_allocator<T>::deallocate


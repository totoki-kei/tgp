#pragma once
#include "Resource.h"

template<typename T>
class ResourceItem : public Resource {
	T item;
	std::function< void(T&) > disposer;
	bool disposed;

public:
	T& get() { return T; }

	template <typename TCallback>
	ResourceItem(T i, TCallback dispose_fn) :
		item(i),
		disposer(dispose_fn),
		disposed(false) {
#ifdef UNICODE
		RSC_DBG_OUT("ResourceItem #%d as %S(size = %d)\n", this->ResourceId, typeid(T).name(), sizeof(T));
#else
		RSC_DBG_OUT("ResourceItem #%d as %s(size = %d)\n", this->ResourceId, typeid(T).name(), sizeof(T));
#endif
	}

	ResourceItem(T p) :
		item(p),
		disposer([](T& i){ delete i; i = nullptr; }),
		disposed(false) {
#ifdef UNICODE
		RSC_DBG_OUT("ResourceItem #%d as %S(size = %d)\n", this->ResourceId, typeid(T).name(), sizeof(T));
#else
		RSC_DBG_OUT("ResourceItem #%d as %s(size = %d)\n", this->ResourceId, typeid(T).name(), sizeof(T));
#endif
	}

	~ResourceItem(){
		if (!isDisposed()) Dispose();
	}

	virtual bool isDisposed() { return disposed; }
	virtual void Dispose() {
		if (!isDisposed()) {
			RSC_DBG_OUT("ResourceItem #%d dispose start.\n", this->ResourceId);
			disposer(item);
			disposed = true;
			Resource::Dispose();
			RSC_DBG_OUT("ResourceItem #%d dispose finished.\n", this->ResourceId);
		}
		else {
			RSC_DBG_OUT("ResourceItem #%d is already disposed.\n", this->ResourceId);
		}
	}
};

template <typename T>
inline std::shared_ptr< ResourceItem<T*> > PtrToRes(T* p){
	auto res = new ResourceItem<T*>(p);
#ifdef UNICODE
	RSC_DBG_OUT("Pointer %p of type %S -> Resource #%d.\n", p, typeid(T).name(), res->GetResourceID());
#else
	RSC_DBG_OUT("Pointer %p of type %s -> Resource #%d.\n", p, typeid(T).name(), res->GetResourceID());
#endif
	return std::shared_ptr< ResourceItem<T*> >(res);
}

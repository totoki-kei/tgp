#pragma once

#define RSC_DBG

#include <memory>
#include <vector>
#include <functional>

#include "Debug.h"
#include "WinAPI.h"

#ifdef RSC_DBG
#define RSC_DBG_OUT DBG_OUT
#else
#define RSC_DBG_OUT __noop
#endif


class Resource{
protected:
	std::vector< std::shared_ptr<Resource> > children;
	unsigned int ResourceId;

public:
	void AddResource(std::shared_ptr<Resource>);
	bool RemoveResource(std::shared_ptr<Resource>, bool all = false, bool recursive = false);
	unsigned int GetResourceID() const;
	std::weak_ptr<Resource> FindResource(unsigned int id, bool recursive = false);
	
	
	Resource(void);
	Resource(unsigned int);
	virtual bool isDisposed();
	virtual void Dispose();
	virtual ~Resource();

private:
	static unsigned int nextid;

};

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

		RSC_DBG_OUT("ResourceItem #%d as %s(size = %d)\n", this->ResourceId, typeid(T).name(), sizeof(T));
	}

	ResourceItem(T p) :
		item(p),
		disposer([](T& i){
			delete i;
			i = nullptr;
		}),
		disposed(false) {

		RSC_DBG_OUT("ResourceItem #%d as %s(size = %d)\n", this->ResourceId, typeid(T).name(), sizeof(T));
	}

	virtual bool isDisposed() { return disposed; }
	virtual void Dispose() { 
		if (!isDisposed()) {
			RSC_DBG_OUT("ResourceItem #%d disposing...\n", this->ResourceId);
			Resource::Dispose();
			disposer (item);
			disposed = true;
			RSC_DBG_OUT("ResourceItem #%d disposed.\n", this->ResourceId);
		}
		else {
			RSC_DBG_OUT("ResourceItem #%d is already disposed.\n", this->ResourceId);
		}
	}
};

template <typename T>
inline std::shared_ptr< ResourceItem<T*> > PtrToRes(T* p){ 
	auto res = new ResourceItem<T*>(p);
	RSC_DBG_OUT("Pointer %p of type %s -> Resource #%d.\n", p, typeid(T).name(), res->GetResourceID());
	return std::shared_ptr< ResourceItem<T*> >(res);
}

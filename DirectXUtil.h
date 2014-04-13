#pragma once

#include "Resource.h"
#include "ResourceItem.h"

#include "Debug.h"

//#define HND_DBG


#ifdef HND_DBG
#define HND_DBG_OUT DBG_OUT
#else
#define HND_DBG_OUT __noop
#endif


#ifndef IF_OK
#define IF_OK(cond) if(SUCCEEDED(cond))
#define IF_OK2(cond, hr) if(SUCCEEDED( hr = (cond)))
#endif

#ifndef IF_NG
#define IF_NG(cond) if(FAILED(cond))
#define IF_NG2(cond, hr) if(FAILED( hr = (cond)))
#endif


template <typename THandle>
inline void ReleaseHandle(THandle*& h){
	h->Release();
	HND_DBG_OUT("handle %s(%p) released\n", typeid(THandle).name(), h);
}


template <typename THandle>
inline std::shared_ptr< ResourceItem<THandle*> > HndToRes(THandle* p){ 
	return std::shared_ptr< ResourceItem<THandle*> >(new ResourceItem<THandle*>(p, ReleaseHandle<THandle> ));
}

template <typename THandle>
inline void NameToResource(THandle* handle, const char* name){
	handle->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name) + 1, name);
}

template <>
inline void NameToResource<IDXGIObject>(IDXGIObject* handle, const char* name){
	handle->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name) + 1, name);
}

template <>
inline void NameToResource<ID3D11DeviceChild>(ID3D11DeviceChild* handle, const char* name){
	handle->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name) + 1, name);
}

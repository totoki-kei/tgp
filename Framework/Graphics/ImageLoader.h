#pragma once

#include "D3DHeaders.h"

class ImageData
{

	DWORD width;
	DWORD height;
	DWORD stride;
	DXGI_FORMAT pixelFormat;
	DWORD bytesPerPixel;
	DWORD dataLength;
	BYTE* data;
	
	inline ImageData(DWORD w, DWORD h, DWORD str, DXGI_FORMAT fmt, DWORD pbytes);
public:
	inline ~ImageData() { delete[] data; }

	static ImageData* Load(const TCHAR* filename);

	inline DWORD GetWidth(){ return width; }
	inline DWORD GetHeight(){ return height; }
	inline DWORD GetStride(){ return stride; }
	inline DXGI_FORMAT GetFormat(){ return pixelFormat; }
	inline DWORD GetBytesPerPixel(){ return bytesPerPixel; }
	inline DWORD GetDataSize(){ return dataLength; }
	inline BYTE* GetData(){ return data; }
};


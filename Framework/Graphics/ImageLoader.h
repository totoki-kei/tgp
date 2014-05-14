#pragma once

#include "D3DHeaders.h"

class ImageData
{

	int width;
	int height;
	DXGI_FORMAT format;
	BYTE* data;
	
	inline ImageData() : width{ 0 }, height{ 0 }, format{ DXGI_FORMAT_UNKNOWN }, data{ nullptr } { }
public:
	inline ~ImageData() { delete[] data; }

	static ImageData* Load(const TCHAR* filename);

	inline int GetWidth(){ return width; }
	inline int GetHeight(){ return height; }
	inline DXGI_FORMAT GetFormat(){ return format; }
	inline BYTE* GetData(){ return data; }
};


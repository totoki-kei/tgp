#pragma once

#include "D3DHeaders.h"

class ImageData
{

	int width;
	int height;
	BYTE* data;
	
	inline ImageData() : width{ 0 }, height{ 0 }, data{ nullptr } { }
public:
	inline ~ImageData() { delete[] data; }

	static ImageData* Load(const TCHAR* filename);

	inline int GetWidth(){ return width; }
	inline int GetHeight(){ return height; }
	inline DXGI_FORMAT GetFormat(){ return DXGI_FORMAT_B8G8R8A8_UNORM; }
	inline BYTE* GetData(){ return data; }
};


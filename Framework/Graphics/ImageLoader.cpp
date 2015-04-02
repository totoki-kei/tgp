#undef NOMINMAX

#include "ImageLoader.h"

#include "../Utility.h"


#include <tchar.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

using Gdiplus::GdiplusStartupInput;
using Gdiplus::Bitmap;
using Gdiplus::BitmapData;
using Gdiplus::ColorPalette;
using Gdiplus::ARGB;

ImageData::ImageData(DWORD w, DWORD h, DWORD str, DXGI_FORMAT fmt, DWORD pbytes){
	this->width = w;
	this->height = h;
	this->stride = str;
	this->pixelFormat = fmt;
	this->bytesPerPixel = pbytes;

	this->dataLength = bytesPerPixel * stride * height;
	this->data = new BYTE[dataLength];
}


ImageData* ImageData::Load(const TCHAR* filename){
	ULONG_PTR token;
	GdiplusStartupInput input;

	ImageData* ret = nullptr;

	Gdiplus::GdiplusStartup(&token, &input, nullptr);
	{

		Bitmap* bmp;
		{
			wchar_t* wcsname;
			auto clen = GetWideCharStringLength(filename) + 1;
			wcsname = new wchar_t[clen];

			ConvertStringWidth(wcsname, clen, filename);
			bmp = Bitmap::FromFile(wcsname);
			delete[] wcsname;
		}
		{

			int width = bmp->GetWidth();
			int height = bmp->GetHeight();
			auto format = bmp->GetPixelFormat();

			BitmapData bmpData;
			bmp->LockBits(nullptr, 0, PixelFormat32bppARGB, &bmpData); // 全要素8ビットの形式で読む
			{
				ret = new ImageData(width, height, width * sizeof(ARGB), DXGI_FORMAT_B8G8R8A8_UNORM, sizeof(ARGB));

				BYTE *data = (BYTE*)ret->data;

				{
					BYTE *dst = data;
					BYTE* scan0 = (BYTE*)bmpData.Scan0;
					int stride = bmpData.Stride < 0 ? -bmpData.Stride : bmpData.Stride;
					for (UINT y = 0; y < bmpData.Height; y++){
						BYTE* scan = scan0 + stride * y;
						CopyMemory(dst, scan, sizeof(ARGB) * bmpData.Width);

						dst += sizeof(ARGB) * bmpData.Width;
					}

				}
			}
			bmp->UnlockBits(&bmpData);
		}
		delete bmp;

	}
	Gdiplus::GdiplusShutdown(token);

	return ret;
}

ImageData::~ImageData(){
	delete[] data;
}

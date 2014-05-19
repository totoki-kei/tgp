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


ImageData* ImageData::Load(const TCHAR* filename){
	ULONG_PTR token;
	GdiplusStartupInput input;

	Gdiplus::GdiplusStartup(&token, &input, nullptr);

	wchar_t* wcsname;
	{
		int clen = _tcslen(filename);
		wcsname = new wchar_t[clen];

		ConvertStringWidth(wcsname, clen, filename);
	}
	auto bmp = Bitmap::FromFile(wcsname);
	delete[] wcsname;

	int width = bmp->GetWidth();
	int height = bmp->GetHeight();
	auto format = bmp->GetPixelFormat();
	ColorPalette* paletteArray = nullptr;
	if (Gdiplus::IsIndexedPixelFormat(format)){
		int paletteSize = bmp->GetPaletteSize();
		paletteArray = (ColorPalette*)malloc(sizeof (Gdiplus::ColorPalette)+sizeof(ARGB)* (paletteSize - 1));
		bmp->GetPalette(paletteArray, paletteSize);
	}

	BitmapData bmpData;
	bmp->LockBits(nullptr, 0, PixelFormat32bppARGB, &bmpData); // 全要素8ビットの形式で読む
	

	

	XMBYTE4 *data = new XMBYTE4[sizeof(XMBYTE4) * bmpData.Width * bmpData.Height];

	{
		XMBYTE4 *dst = data;
		BYTE* scan0 = (BYTE*)bmpData.Scan0 + bmpData.Stride * (height - 1);
		int stride = bmpData.Stride < 0 ? -bmpData.Stride : bmpData.Stride;
		for (int h = 0; h < bmpData.Height; h++){
			BYTE* scan = scan0 + stride * h;
			for (int w = 0; w < bmpData.Width; w++){
				// ピクセルフォーマットの変換(ARGB -> RGBA)
				// TODO 後でテストして変換方法が正しいか確認する
				ARGB* argb = (ARGB*)scan;
				dst->v = ((*argb << 8) & 0xFFFFFF00) | ((*argb >> 24) & 0x000000FF);

				dst++;
				scan += sizeof(ARGB);
			}
		}
	}

	bmp->UnlockBits(&bmpData);


	auto ret = new ImageData();
	ret->width = width;
	ret->height = height;
	ret->data = (BYTE*)data;


	TERMINATE:
	if (paletteArray) free(paletteArray);
	delete bmp;
	Gdiplus::GdiplusShutdown(token);

	return ret;
}

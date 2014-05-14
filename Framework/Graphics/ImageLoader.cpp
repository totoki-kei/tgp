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

struct	Px1bppIndexed	{ unsigned int N : 1;					enum { SizeN = 1, SizeA = 0, SizeR = 0, SizeG = 0, SizeB = 0, SizeTotal = 1, };	int GetN() { return (int)N; }	int GetA() { return -1; }	int GetR() { return -1; }	int GetG() { return -1; }	int GetB() { return -1; }	static const DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM;	static const int FormatSize = 32; };
struct	Px4bppIndexed	{ unsigned int N : 4;					enum { SizeN = 4, SizeA = 0, SizeR = 0, SizeG = 0, SizeB = 0, SizeTotal = 4, };	int GetN() { return (int)N; }	int GetA() { return -1; }	int GetR() { return -1; }	int GetG() { return -1; }	int GetB() { return -1; }	static const DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM;	static const int FormatSize = 32; };
struct	Px8bppIndexed	{ unsigned int N : 8;					enum { SizeN = 8, SizeA = 0, SizeR = 0, SizeG = 0, SizeB = 0, SizeTotal = 8, };	int GetN() { return (int)N; }	int GetA() { return -1; }	int GetR() { return -1; }	int GetG() { return -1; }	int GetB() { return -1; }	static const DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM;	static const int FormatSize = 32; };
struct	Px16bppGrayScale	{ unsigned int N : 16;					enum { SizeN = 16, SizeA = 0, SizeR = 0, SizeG = 0, SizeB = 0, SizeTotal = 16, };	int GetN() { return (int)N; }	int GetA() { return -1; }	int GetR() { return -1; }	int GetG() { return -1; }	int GetB() { return -1; }	static const DXGI_FORMAT Format = DXGI_FORMAT_R16_UNORM;	static const int FormatSize = 16; };
struct	Px16bppRGB555	{ unsigned int N : 1;		unsigned int R : 5;	unsigned int G : 5;	unsigned int B : 5;	enum { SizeN = 1, SizeA = 0, SizeR = 5, SizeG = 5, SizeB = 5, SizeTotal = 16, };	int GetN() { return -1; }	int GetA() { return -1; }	int GetR() { return (int)R; }	int GetG() { return (int)G; }	int GetB() { return (int)B; }	static const DXGI_FORMAT Format = DXGI_FORMAT_B5G5R5A1_UNORM;	static const int FormatSize = 16; };
struct	Px16bppRGB565	{ unsigned int R : 5;	unsigned int G : 6;	unsigned int B : 5;	enum { SizeN = 0, SizeA = 0, SizeR = 5, SizeG = 6, SizeB = 5, SizeTotal = 16, };	int GetN() { return -1; }	int GetA() { return -1; }	int GetR() { return (int)R; }	int GetG() { return (int)G; }	int GetB() { return (int)B; }	static const DXGI_FORMAT Format = DXGI_FORMAT_B5G6R5_UNORM;	static const int FormatSize = 16; };
struct	Px16bppARGB1555	{ unsigned int A : 1;	unsigned int R : 5;	unsigned int G : 5;	unsigned int B : 5;	enum { SizeN = 0, SizeA = 1, SizeR = 5, SizeG = 5, SizeB = 5, SizeTotal = 16, };	int GetN() { return -1; }	int GetA() { return (int)A; }	int GetR() { return (int)R; }	int GetG() { return (int)G; }	int GetB() { return (int)B; }	static const DXGI_FORMAT Format = DXGI_FORMAT_B5G5R5A1_UNORM;	static const int FormatSize = 16; };
struct	Px24bppRGB	{ unsigned int R : 8;	unsigned int G : 8;	unsigned int B : 8;	enum { SizeN = 0, SizeA = 0, SizeR = 8, SizeG = 8, SizeB = 8, SizeTotal = 24, };	int GetN() { return -1; }	int GetA() { return -1; }	int GetR() { return (int)R; }	int GetG() { return (int)G; }	int GetB() { return (int)B; }	static const DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM;	static const int FormatSize = 32; };
struct	Px32bppRGB	{ unsigned int N : 8;		unsigned int R : 8;	unsigned int G : 8;	unsigned int B : 8;	enum { SizeN = 8, SizeA = 0, SizeR = 8, SizeG = 8, SizeB = 8, SizeTotal = 32, };	int GetN() { return -1; }	int GetA() { return -1; }	int GetR() { return (int)R; }	int GetG() { return (int)G; }	int GetB() { return (int)B; }	static const DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM;	static const int FormatSize = 32; };
struct	Px32bppARGB	{ unsigned int A : 8;	unsigned int R : 8;	unsigned int G : 8;	unsigned int B : 8;	enum { SizeN = 0, SizeA = 8, SizeR = 8, SizeG = 8, SizeB = 8, SizeTotal = 32, };	int GetN() { return -1; }	int GetA() { return (int)A; }	int GetR() { return (int)R; }	int GetG() { return (int)G; }	int GetB() { return (int)B; }	static const DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM;	static const int FormatSize = 32; };
struct	Px32bppPARGB	{ unsigned int A : 8;	unsigned int R : 8;	unsigned int G : 8;	unsigned int B : 8;	enum { SizeN = 0, SizeA = 8, SizeR = 8, SizeG = 8, SizeB = 8, SizeTotal = 32, };	int GetN() { return -1; }	int GetA() { return (int)A; }	int GetR() { return (int)R; }	int GetG() { return (int)G; }	int GetB() { return (int)B; }	static const DXGI_FORMAT Format = DXGI_FORMAT_R8G8B8A8_UNORM;	static const int FormatSize = 32; };
struct	Px48bppRGB	{ unsigned int R : 16;	unsigned int G : 16;	unsigned int B : 16;	enum { SizeN = 0, SizeA = 0, SizeR = 16, SizeG = 16, SizeB = 16, SizeTotal = 48, };	int GetN() { return -1; }	int GetA() { return -1; }	int GetR() { return (int)R; }	int GetG() { return (int)G; }	int GetB() { return (int)B; }	static const DXGI_FORMAT Format = DXGI_FORMAT_R16G16B16A16_UNORM;	static const int FormatSize = 64; };
struct	Px64bppARGB	{ unsigned int A : 16;	unsigned int R : 16;	unsigned int G : 16;	unsigned int B : 16;	enum { SizeN = 0, SizeA = 16, SizeR = 16, SizeG = 16, SizeB = 16, SizeTotal = 64, };	int GetN() { return -1; }	int GetA() { return (int)A; }	int GetR() { return (int)R; }	int GetG() { return (int)G; }	int GetB() { return (int)B; }	static const DXGI_FORMAT Format = DXGI_FORMAT_R16G16B16A16_UNORM;	static const int FormatSize = 64; };
struct	Px64bppPARGB	{ unsigned int A : 16;	unsigned int R : 16;	unsigned int G : 16;	unsigned int B : 16;	enum { SizeN = 0, SizeA = 16, SizeR = 16, SizeG = 16, SizeB = 16, SizeTotal = 64, };	int GetN() { return -1; }	int GetA() { return (int)A; }	int GetR() { return (int)R; }	int GetG() { return (int)G; }	int GetB() { return (int)B; }	static const DXGI_FORMAT Format = DXGI_FORMAT_R16G16B16A16_UNORM;	static const int FormatSize = 64; };
//struct	Px32bppCMYK	{ unsigned int N : 32;					enum { SizeN = 32, SizeA = 0, SizeR = 0, SizeG = 0, SizeB = 0, SizeTotal = 32, };	int GetN() { return (int)N; }	int GetA() { return -1; }	int GetR() { return -1; }	int GetG() { return -1; }	int GetB() { return -1; }	static const DXGI_FORMAT Format = ; };


template <typename PixelT>
BYTE* Decode(BitmapData* bmp, ColorPalette* palette){
	// PixelT::SizeTotal >= 8 の時の関数。8ビットより小さい場合は特殊化して別途定義する

	PixelT * src = (PixelT*)bmp->Scan0;
	int srcBytes = Gdiplus::GetPixelFormatSize(bmp->PixelFormat) * bmp->Width * bmp->Height / 8;

	int dstBytes = PixelT::FormatSize * bmp->Width * bmp->Height / 8;	
	BYTE* dst = new BYTE[dstBytes];

	

	return dst;
}

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

	DXGI_FORMAT dxgiFormat = DXGI_FORMAT_UNKNOWN;

	BitmapData bmpData;
	bmp->LockBits(nullptr, 0, format, &bmpData);
	

	switch (format){
	case PixelFormat16bppRGB555:    	
	case PixelFormat16bppARGB1555:
	case PixelFormat16bppRGB565:
	case PixelFormat16bppGrayScale:
	case PixelFormat48bppRGB:
	case PixelFormat64bppARGB:
	case PixelFormat64bppPARGB:
	case PixelFormat1bppIndexed:
	case PixelFormat4bppIndexed:
	case PixelFormat8bppIndexed:
	case PixelFormat24bppRGB:
	case PixelFormat32bppRGB:
	case PixelFormat32bppARGB:
	case PixelFormat32bppPARGB:
	default:
		// not supported
		goto TERMINATE;
	}



	BYTE *data = nullptr;


	auto ret = new ImageData();
	ret->width = width;
	ret->height = height;
	ret->format = dxgiFormat;
	ret->data = data;


	TERMINATE:
	if (paletteArray) free(paletteArray);
	delete bmp;
	Gdiplus::GdiplusShutdown(token);

	return ret;
}

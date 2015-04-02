#include "Utility.h"

#include <new>
#include <stdexcept>
#include <mbstring.h>

size_t ConvertStringWidth(char *dst, size_t dstSize, const char *src) {
	_mbscpy_s((unsigned char*)dst, dstSize, (const unsigned char*)src);
	return _mbstrlen(dst);
}
size_t ConvertStringWidth(char *dst, size_t dstSize, const wchar_t *src) {
	int bytes = WideCharToMultiByte(
		CP_ACP,
		0,
		src, -1,
		dst, (int)dstSize,
		nullptr, nullptr);
	return bytes;
}
size_t ConvertStringWidth(wchar_t *dst, size_t dstSize, const char *src) {
	int words = MultiByteToWideChar(
		CP_ACP,
		0,
		src, -1,
		dst, (int)dstSize);

	return words;
}
size_t ConvertStringWidth(wchar_t *dst, size_t dstSize, const wchar_t *src) {
	wcscpy_s(dst, dstSize, src);
	return wcslen(dst);
}

size_t GetWideCharStringLength(const char* txt) {
	return (size_t)MultiByteToWideChar(
		CP_ACP,
		0,
		txt, -1,
		nullptr, -1);
}
size_t GetWideCharStringLength(const wchar_t* txt) {
	return wcslen(txt);
}
size_t GetMultibyteStirngLength(const char* txt) {
	return _mbstrlen(txt);
}
size_t GetMultibyteStirngLength(const wchar_t* txt) {
	return (size_t)WideCharToMultiByte(
		CP_ACP,
		0,
		txt, -1,
		nullptr, -1,
		nullptr, nullptr);
}


BYTE* LoadFileToMemory(const TCHAR* filename, /* out */ int *size){
	auto hnd = CreateFile(filename, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hnd == INVALID_HANDLE_VALUE) {
		// ファイルを開けなかったのでダメ
		return nullptr;
	}

	DWORD sizeL, sizeH = 0;
	sizeL = GetFileSize(hnd, &sizeH);
	if (sizeH > 0){
		// ファイル大きすぎてダメ
		return nullptr;
	}

	int srcSize = sizeL;

	BYTE *buffer = new BYTE[srcSize];

	DWORD readBytes = 0;
	ReadFile(hnd, buffer, srcSize, &readBytes, nullptr);
	CloseHandle(hnd);

	*size = srcSize;
	return buffer;
}

using DirectX::XMVECTOR;

float CalcDistanceLineToPoint(const XMVECTOR& a, const XMVECTOR& b, const XMVECTOR &p) {
	using namespace DirectX;

	if (XMVectorGetX(XMVector2Dot(XMVectorSubtract(b, a), XMVectorSubtract(p, a))) < 0) {
		// 点aのなす角が鈍角(内積が負) -> aとpの距離を使う
		return XMVectorGetX(XMVector2Length(XMVectorSubtract(a, p)));
	}
	else if (XMVectorGetX(XMVector2Dot(XMVectorSubtract(a, b), XMVectorSubtract(p, b))) < 0) {
		// 点bのなす角が鈍角(内積が負) -> bとpの距離を使う
		return XMVectorGetX(XMVector2Length(XMVectorSubtract(b, p)));
	}
	else {
		// 点と直線の距離を使う
		return XMVectorGetX(XMVector2LinePointDistance(a, b, p));
	}
}

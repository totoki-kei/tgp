
#include "Utility.h"

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


size_t ConvertStringWidth(char *dst, int dstSize, const char *src){
	strcpy_s(dst, dstSize, src);
	return strlen(dst);
}
size_t ConvertStringWidth(char *dst, int dstSize, const wchar_t *src){
	int bytes = WideCharToMultiByte(
		CP_ACP,
		0,
		src, wcslen(src),
		dst, dstSize,
		nullptr, nullptr);
	return bytes;
}
size_t ConvertStringWidth(wchar_t *dst, int dstSize, const char *src){
	int words = MultiByteToWideChar(
		CP_ACP,
		0,
		src, strlen(src),
		dst, dstSize);

	return words;
}
size_t ConvertStringWidth(wchar_t *dst, int dstSize, const wchar_t *src){
	wcscpy_s(dst, dstSize, src);
	return wcslen(dst);
}


#include "Utility.h"

BYTE* LoadFileToMemory(const TCHAR* filename, /* out */ int *size){
	auto hnd = CreateFile(filename, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hnd == INVALID_HANDLE_VALUE) {
		// �t�@�C�����J���Ȃ������̂Ń_��
		return nullptr;
	}

	DWORD sizeL, sizeH = 0;
	sizeL = GetFileSize(hnd, &sizeH);
	if (sizeH > 0){
		// �t�@�C���傫�����ă_��
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

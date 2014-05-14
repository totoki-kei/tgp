#pragma once

#include "WinAPI.h"

BYTE* LoadFileToMemory(const TCHAR* filename, /* out */ int *size);

size_t ConvertStringWidth(char *dst, int dstSize, const char *src);
size_t ConvertStringWidth(char *dst, int dstSize, const wchar_t *src);
size_t ConvertStringWidth(wchar_t *dst, int dstSize, const char *src);
size_t ConvertStringWidth(wchar_t *dst, int dstSize, const wchar_t *src);

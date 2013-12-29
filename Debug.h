#pragma once

#include <tchar.h>

#ifdef _DEBUG
#include <stdlib.h>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

void PrintDebug(const TCHAR* format, ...);
void PrintDebug(const TCHAR* filename, int line, const TCHAR* fn, const TCHAR* format, ...);
#ifdef _DEBUG
#define DBG_OUT(f, ...) PrintDebug(__FILE__, __LINE__, __FUNCTION__, f, __VA_ARGS__)
#else
#define DBG_OUT __noop
#endif


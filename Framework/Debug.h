#pragma once

#include <tchar.h>

#ifdef _DEBUG
#include <stdlib.h>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

//void PrintDebug(const TCHAR* format, ...);
void PrintDebug(const TCHAR* filename, int line, const TCHAR* fn, const char* format, ...);
#ifdef _DEBUG
#ifdef UNICODE
#define DBG_OUT(f, ...) PrintDebug(__FILEW__, __LINE__, __FUNCTIONW__, f, __VA_ARGS__)
#else
#define DBG_OUT(f, ...) PrintDebug(__FILE__, __LINE__, __FUNCTION__, f, __VA_ARGS__)
#endif
#else
#define DBG_OUT __noop
#endif


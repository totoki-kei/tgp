#pragma once

#include <tchar.h>

#ifdef _DEBUG
#include <stdlib.h>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif


void SetLogfileName(const TCHAR* filename);

void SetOutputLogLevel(const char* level, bool dbgwindow);

//void PrintDebug(const TCHAR* format, ...);
void PrintDebug(TCHAR level, const TCHAR* filename, int line, const TCHAR* fn, const TCHAR* format, ...);
//#ifdef _DEBUG
#ifdef UNICODE
#define LOG(level, fmt, ...) PrintDebug(level, __FILEW__, __LINE__, __FUNCTIONW__, _T ( fmt ), __VA_ARGS__)
#else
#define LOG(level, fmt, ...) PrintDebug(level, __FILE__, __LINE__, __FUNCTION__,  _T ( fmt ), __VA_ARGS__)
#endif

#define LOG_TRACE(fmt, ...) LOG(_T('T'), fmt, __VA_ARGS__)
#define LOG_DBG(fmt, ...)   LOG(_T('D'), fmt, __VA_ARGS__)
#define LOG_INFO(fmt, ...)  LOG(_T('I'), fmt, __VA_ARGS__)
#define LOG_WARN(fmt, ...)  LOG(_T('W'), fmt, __VA_ARGS__)
#define LOG_ERR(fmt, ...)   LOG(_T('E'), fmt, __VA_ARGS__)
#define LOG_CRIT(fmt, ...)  LOG(_T('C'), fmt, __VA_ARGS__)


#include <cstdio>
#include <cstdarg>

#include "WinAPI.h"

#include "Debug.h"

void PrintDebug(const TCHAR* format, ...){
	va_list	args;
	va_start(args, format);

	TCHAR buff[256];
	_vstprintf_s(buff, format, args);
	OutputDebugString(buff);

	va_end(args);
}

void PrintDebug(const TCHAR* filename, int line, const TCHAR* fn, const TCHAR* format, ...){
	va_list	args;
	va_start(args, format);

	TCHAR buff[256];
	_vstprintf_s(buff, format, args);

	TCHAR buff2[256];
	_stprintf_s(buff2, _T("%10d %s:%d [%s] %s"), GetTickCount(), filename, line, fn, buff);
	OutputDebugString(buff2);

	va_end(args);
}

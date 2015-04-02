
#include <cstdio>
#include <cstdarg>

#include "WinAPI.h"

#include "Debug.h"
#include "Utility.h"

#include <memory>
#include <fstream>

//void PrintDebug(const TCHAR* format, ...){
//	va_list	args;
//	va_start(args, format);
//
//	TCHAR buff[256];
//	_vstprintf_s(buff, format, args);
//	OutputDebugString(buff);
//
//	va_end(args);
//}

namespace {
	typedef std::basic_fstream<TCHAR> tfstream;

	const char* loglevel = "CEW";
	bool debugout = true;
	std::unique_ptr<tfstream> stream;
}

void SetLogfileName(const TCHAR* filename) {
	stream = std::make_unique<tfstream>(filename, std::ios::app);
}

void SetOutputLogLevel(const char* level, bool debugwindow) {
	loglevel = level ? level : "";
	debugout = debugwindow;
}


const int BufferSize = 1024;
void PrintDebug(const TCHAR level, const TCHAR* filename, int line, const TCHAR* fn, const TCHAR* format, ...) {
	bool fileout;
#ifdef _DEBUG
	// デバッグコンパイル時は、出力対象ログレベルの時にのみファイル出力する
	// デバッグウィンドウには必ず出力する
	fileout = strchr(loglevel, level) != nullptr;
#else
	// リリースコンパイル時は、出力対象ログレベルの時にのみファイル、デバッグウィンドウに出力する
	// (対象でない場合はそもそも文字列生成をしないようにするため)
	if (!strchr(loglevel, level)) return;
	fileout = true;
#endif

	SYSTEMTIME systemtime = { 0 };
	GetLocalTime(&systemtime);

	va_list	args;
	va_start(args, format);

	TCHAR text[BufferSize] = { 0 };
	TCHAR timestr[32] = { 0 };
	_stprintf_s(timestr,
				_T("%04d/%02d/%02d %02d:%02d:%02d.%03d"),
				systemtime.wYear, systemtime.wMonth, systemtime.wDay,
				systemtime.wHour, systemtime.wMinute, systemtime.wSecond, systemtime.wMilliseconds);


	{
		TCHAR buff[BufferSize];

		_vstprintf_s(buff, format, args);

		auto outfmt = _T("%c\t%s\t%s:%d\t[%s]\t%s");
		_stprintf_s(text, outfmt, level, timestr, filename, line, fn, buff);
	}

	if (stream && fileout) *stream << (const TCHAR*)text;
	if (debugout) OutputDebugString(text);

	va_end(args);
}
//
//void* operator new(size_t size){
//	void* ptr = malloc(size);
//	LOG_DBG("addr = 0x%X, size = %d\n", ptr, size);
//	return ptr;
//}
//
//void operator delete(void* p){
//	LOG_DBG("addr = 0x%X\n", p);
//	free(p);
//}
//

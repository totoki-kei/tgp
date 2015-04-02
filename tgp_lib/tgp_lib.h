#pragma once

#ifdef INTERNAL
#ifdef DLL_IMPL
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI
#endif
#else
#define DLLAPI __declspec(dllimport)
#pragma comment(lib, "tgp_lib.lib")
#endif

#include <errno.h>

namespace tgp_lib{

	size_t DLLAPI ConvertStringWidth(char *dst, size_t dstSize, const char *src);
	size_t DLLAPI ConvertStringWidth(char *dst, size_t dstSize, const wchar_t *src);
	size_t DLLAPI ConvertStringWidth(wchar_t *dst, size_t dstSize, const char *src);
	size_t DLLAPI ConvertStringWidth(wchar_t *dst, size_t dstSize, const wchar_t *src);

	size_t DLLAPI GetMultibyteStirngLength(const char* txt);
	size_t DLLAPI GetMultibyteStirngLength(const wchar_t* txt);
	size_t DLLAPI GetWideCharStringLength(const char* txt);
	size_t DLLAPI GetWideCharStringLength(const wchar_t* txt);

	struct Config {
		bool fullscreen;
		unsigned int screenWidth;
		unsigned int screenHeight;

		struct GameInput{
			bool useDigitalPad;
			unsigned int action;
			unsigned int subaction;
			unsigned int rotateLeft;
			unsigned int rotateRight;
			unsigned int menu;
		} keyboard, gamepad;

	};

	void DLLAPI InitConfig(Config*);

	errno_t DLLAPI LoadConfig(Config*, const wchar_t* filepath);
	errno_t DLLAPI LoadConfig(Config*, const char* filepath);

	errno_t DLLAPI SaveConfig(Config*, const wchar_t* filepath);
	errno_t DLLAPI SaveConfig(Config*, const char* filepath);


	

}

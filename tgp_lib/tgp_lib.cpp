#include "Stdafx.h"

#define INTERNAL
#define DLL_IMPL
#include "tgp_lib.h"

#include <Windows.h>

#include <string.h>
#include <mbstring.h>
#include <errno.h>

#include <functional>
#include <memory>

namespace {
	class ScopeExit{
		std::function<void()> fn;

	public:
		template <typename Fn>
		ScopeExit(Fn callback) : fn{ callback } { }

		~ScopeExit() {
			fn();
		}
	};
}


namespace tgp_lib {
	size_t DLLAPI ConvertStringWidth(char *dst, size_t dstSize, const char *src){
		_mbscpy_s((unsigned char*)dst, dstSize, (const unsigned char*)src);
		return _mbstrlen(dst);
	}
	size_t DLLAPI ConvertStringWidth(char *dst, size_t dstSize, const wchar_t *src){
		int bytes = WideCharToMultiByte(
			CP_ACP,
			0,
			src, -1,
			dst, (int)dstSize,
			nullptr, nullptr);
		return bytes;
	}
	size_t DLLAPI ConvertStringWidth(wchar_t *dst, size_t dstSize, const char *src){
		int words = MultiByteToWideChar(
			CP_ACP,
			0,
			src, -1,
			dst, (int)dstSize);

		return words;
	}
	size_t DLLAPI ConvertStringWidth(wchar_t *dst, size_t dstSize, const wchar_t *src){
		wcscpy_s(dst, dstSize, src);
		return wcslen(dst);
	}

	size_t DLLAPI GetWideCharStringLength(const char* txt){
		return (size_t)MultiByteToWideChar(
			CP_ACP,
			0,
			txt, -1,
			nullptr, -1);
	}
	size_t DLLAPI GetWideCharStringLength(const wchar_t* txt) {
		return wcslen(txt);
	}
	size_t DLLAPI GetMultibyteStirngLength(const char* txt) {
		return _mbstrlen(txt);
	}
	size_t DLLAPI GetMultibyteStirngLength(const wchar_t* txt) {
		return (size_t)WideCharToMultiByte(
			CP_ACP,
			0,
			txt, -1,
			nullptr, -1,
			nullptr, nullptr);
	}


	void InitConfig(Config* cfg){
		cfg->fullscreen = false;
		cfg->screenWidth = 800;
		cfg->screenHeight = 600;

		cfg->keyboard.useDigitalPad = true;
		cfg->keyboard.action = 'Z';
		cfg->keyboard.subaction = 'X';
		cfg->keyboard.rotateLeft = 'A';
		cfg->keyboard.rotateRight = 'S';
		cfg->keyboard.menu = VK_ESCAPE;

		cfg->gamepad.useDigitalPad = false;
		cfg->gamepad.action = 1;
		cfg->gamepad.subaction = 2;
		cfg->gamepad.rotateLeft = 5;
		cfg->gamepad.rotateRight = 6;
		cfg->gamepad.menu = 3;
	}

	errno_t LoadConfig(Config* cfg, const wchar_t* filepath){
		FILE* file = nullptr;

		if (auto err = _wfopen_s(&file, filepath, L"rb")){
			return err;
		}

		ScopeExit closeFile{ [&file]() {
			if (file) { fclose(file); file = nullptr; }
		} };

		auto count = fread(cfg, sizeof(Config), 1, file);
		if (count != 1) return EIO;

		return 0;
	}

	errno_t LoadConfig(Config* cfg, const char* filepath){
		auto wclength = GetWideCharStringLength(filepath);
		std::unique_ptr<wchar_t> wcfilepath{ new wchar_t[wclength] };
		ConvertStringWidth(wcfilepath.get(), wclength, filepath);
		return LoadConfig(cfg, wcfilepath.get());
	}

	errno_t SaveConfig(Config* cfg, const wchar_t* filepath){
		FILE* file = nullptr;

		if (auto err = _wfopen_s(&file, filepath, L"wb")){
			return err;
		}

		ScopeExit closeFile{ [&file]() {
			if (file) { fclose(file); file = nullptr; }
		} };

		auto count = fwrite(cfg, sizeof(Config), 1, file);
		if (count != 1) return EIO;

		return 0;
	}

	errno_t SaveConfig(Config* cfg, const char* filepath){
		auto wclength = GetWideCharStringLength(filepath);
		std::unique_ptr<wchar_t> wcfilepath{ new wchar_t[wclength] };
		ConvertStringWidth(wcfilepath.get(), wclength, filepath);
		return SaveConfig(cfg, wcfilepath.get());
	}

}
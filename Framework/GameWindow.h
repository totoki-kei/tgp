#pragma once

#include "WinAPI.h"

#include <string>
#include <map>
#include <functional>

#include "Resource.h"

typedef std::basic_string<
	TCHAR,
	std::char_traits<TCHAR>,
	std::allocator<TCHAR>
> tstring;

typedef std::function<int (UINT, WPARAM, LPARAM)> msgfn_t;

class GameWindow : public Resource
{
public:

	enum class Status {
		NotInitialized,
		OK,
		CannotRegisterWndCls,
		FailedToCreateWnd,
		Closed,
	};

protected:
	static TCHAR WndClassName[];

	HWND hWnd;
	Status stat;
	MSG msg;
	bool closed;

	HCURSOR cursor;
	HICON icon;

	DWORD width, height;
	
	tstring wndTitle;

	static std::map<UINT, msgfn_t > msgmap;

public:

	static int WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

	GameWindow(long width, long height);
	GameWindow(long width, long height, HCURSOR cursor, HICON icon);
	~GameWindow(void);

	void SetWindowTitle(const TCHAR* title);

	template<typename TCallback>
	static inline void SetMessageHandler(UINT msg, TCallback callback) {
		msgfn_t fn = callback;
		msgmap[msg] = fn;
	}

	static inline void ResetMessageHandler(UINT msg) {
		msgmap.erase(msg);
	}

	void Initialize();

	int ProcessMessage();

	bool isDisposed();
	void Dispose();

	inline DWORD GetWidth() { return width; }
	inline DWORD GetHeight() { return height; }
	inline HWND GetWindowHandle() {return hWnd; }


};


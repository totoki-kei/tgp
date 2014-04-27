<<<<<<< HEAD:GameWindow.cpp
#include "GameWindow.h"
#include "Program.h"

#include "Debug.h"

TCHAR GameWindow::WndClassName[] = TEXT("TGP_D10_GAMEWND");
std::map<UINT, msgfn_t > GameWindow::msgmap;

int WINAPI GameWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if(msg == WM_DESTROY){
		PostQuitMessage(0);
	}
	else {
		auto iter = msgmap.find(msg);
		if(iter != msgmap.end()) {
			auto fn = iter->second;
			fn(msg, wParam, lParam);
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

GameWindow::GameWindow(long width, long height) :
	Resource(), wndTitle(TEXT("GameWindow")) {
	this->width = width;
	this->height = height;
	this->stat = Status::NotInitialized;
}

void GameWindow::Initialize(void) {
		// ウィンドウクラスの作成
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)(GameWindow::WndProc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_MainArgs.hInstance;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = GameWindow::WndClassName;

	// ウィンドウクラスの登録
	auto ret = RegisterClass(&wc);
	if(!ret) {
		// ウィンドウクラスの登録失敗
		this->stat = Status::CannotRegisterWndCls;
		return;
	}

	DWORD wndStyle = WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	// ウィンドウのふちを考慮したウィンドウサイズを取得
	RECT rect;
	rect.top = rect.left = 0;
	rect.right = width;
	rect.bottom = height;
	AdjustWindowRect(&rect, wndStyle, FALSE);

	// ウィンドウを作成
	auto hWndRet = CreateWindow(
		WndClassName, 
		wndTitle.c_str(),
		wndStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, g_MainArgs.hInstance, NULL);

	if(hWndRet == NULL){
		this->stat = Status::FailedToCreateWnd;
		return;
	}

	this->hWnd = hWndRet;

	// ウィンドウを表示
	ShowWindow(hWnd, g_MainArgs.nShowCmd);
	UpdateWindow(hWnd);

	stat = Status::OK;
	closed = false;
}


GameWindow::~GameWindow(void)
{
	UnregisterClass(WndClassName, g_MainArgs.hInstance);
	DestroyWindow(this->hWnd);
	if (!isDisposed()) Dispose();
}

int GameWindow::ProcessMessage() {

	if(this->stat == Status::Closed) return 1;

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		switch(msg.message) {
		case WM_QUIT:
		case WM_CLOSE:
			this->stat = Status::Closed;
			return 1;
			break;
		}
	}

	return 0;
}

void GameWindow::SetWindowTitle(const TCHAR* title) {
	this->wndTitle = title;

	if(stat == Status::OK) {
		SetWindowText(hWnd, wndTitle.c_str());
		UpdateWindow(hWnd);
	}
}

bool GameWindow::isDisposed() {
	return stat != Status::OK;
}

void GameWindow::Dispose() {
	if(isDisposed()) return;
	PostQuitMessage(0);
	Resource::Dispose();
	while( ! ProcessMessage() );
}

=======
#include "GameWindow.h"
#include "Program.h"

#include "Debug.h"

TCHAR GameWindow::WndClassName[] = TEXT("TGP_D11_GAMEWND");
std::map<UINT, msgfn_t > GameWindow::msgmap;

int WINAPI GameWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if(msg == WM_DESTROY){
		PostQuitMessage(0);
	}
	else {
		auto iter = msgmap.find(msg);
		if(iter != msgmap.end()) {
			auto fn = iter->second;
			fn(msg, wParam, lParam);
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

GameWindow::GameWindow(long width, long height) :
	Resource(), wndTitle(TEXT("GameWindow")) {
	this->width = width;
	this->height = height;
	this->icon = nullptr;
	this->cursor = nullptr;
	this->stat = Status::NotInitialized;
}

GameWindow::GameWindow(long width, long height, HCURSOR cursor, HICON icon) :
Resource(), wndTitle(TEXT("GameWindow")) {
	this->width = width;
	this->height = height;
	this->icon = icon;
	this->cursor = cursor;
	this->stat = Status::NotInitialized;
}

void GameWindow::Initialize(void) {
		// ウィンドウクラスの作成
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)(GameWindow::WndProc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_MainArgs.hInstance;
	wc.hIcon = icon;
	wc.hCursor = cursor;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = GameWindow::WndClassName;

	// ウィンドウクラスの登録
	auto ret = RegisterClass(&wc);
	if(!ret) {
		// ウィンドウクラスの登録失敗
		this->stat = Status::CannotRegisterWndCls;
		return;
	}

	DWORD wndStyle = WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	// ウィンドウのふちを考慮したウィンドウサイズを取得
	RECT rect;
	rect.top = rect.left = 0;
	rect.right = width;
	rect.bottom = height;
	AdjustWindowRect(&rect, wndStyle, FALSE);

	// ウィンドウを作成
	auto hWndRet = CreateWindow(
		WndClassName, 
		wndTitle.c_str(),
		wndStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, g_MainArgs.hInstance, NULL);

	if(hWndRet == NULL){
		this->stat = Status::FailedToCreateWnd;
		return;
	}

	this->hWnd = hWndRet;

	// ウィンドウを表示
	ShowWindow(hWnd, g_MainArgs.nShowCmd);
	UpdateWindow(hWnd);

	stat = Status::OK;
	closed = false;
}


GameWindow::~GameWindow(void)
{
	UnregisterClass(WndClassName, g_MainArgs.hInstance);
	DestroyWindow(this->hWnd);
	if (!isDisposed()) Dispose();
}

int GameWindow::ProcessMessage() {

	if(this->stat == Status::Closed) return 1;

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		switch(msg.message) {
		case WM_QUIT:
		case WM_CLOSE:
			this->stat = Status::Closed;
			return 1;
			break;
		}
	}

	return 0;
}

void GameWindow::SetWindowTitle(const TCHAR* title) {
	this->wndTitle = title;

	if(stat == Status::OK) {
		SetWindowText(hWnd, wndTitle.c_str());
		UpdateWindow(hWnd);
	}
}

bool GameWindow::isDisposed() {
	return stat != Status::OK;
}

void GameWindow::Dispose() {
	if(isDisposed()) return;
	PostQuitMessage(0);
	Resource::Dispose();
	while( ! ProcessMessage() );
}

>>>>>>> DX11:Framework/GameWindow.cpp

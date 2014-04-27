#pragma once

#include "WinAPI.h"

struct MainArgs {
	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	LPSTR lpCmdLine;
	int nShowCmd;
};

extern MainArgs g_MainArgs;

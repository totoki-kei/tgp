#pragma once
#include <cstdint>
#include "Framework/WinAPI.h"

struct Session {
public:
	SYSTEMTIME starttime;
	int random_seed;
	ULONGLONG ticks;
	int items;
	int score;
	Session() {
		GetLocalTime(&starttime);
		random_seed = GetTickCount64();
		ticks = 0;
		items = 0;
		score = 0;
	};
};


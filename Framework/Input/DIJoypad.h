#pragma once

#include "DIHeader.h"

class DIJoypad;


class DIJoypad
{
	int padId;
	bool xinput;
	JOYCAPS joycaps;
public:
	DIJoypad(int padId, bool xinput = true);
	~DIJoypad();

	bool Update(XINPUT_STATE&);
};


#pragma once

#include "../WinAPI.h"
#include <functional>

class DIMouseListener;
enum class DIMouseListenType {
	Position,
	Velocity,
	Acceleration,
};


class DIMouseListener
{
	DIMouseListenType type;

public:
	DIMouseListener();
	~DIMouseListener();
};


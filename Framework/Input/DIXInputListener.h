#pragma once

#include "DIHeader.h"

class DIXInputListener;
enum class DIXInputType{
	Button,
	Axis,
};




class DIXInputListener
{
	int padId;
	bool lastStatus;
public:
	DIXInputListener(int padId);
	~DIXInputListener();

	void Update(XINPUT_STATE&);
};


#include "DIKeyboardListener.h"

DIKeyboardListener::DIKeyboardListener(int vk_key) {
	this->vk = vk_key;
}

int DIKeyboardListener::GetLastStatus(){
	return lastStatus;
}

bool DIKeyboardListener::IsKeyDown(){
	return lastStatus > 0;
}

bool DIKeyboardListener::IsKeyUp(){
	return lastStatus < 0;
}

void DIKeyboardListener::Update(const BYTE(&keys)[256]){
	int stat = keys[vk & 0xFF];
	if (stat & 0x80) {
		// 押されている
		if (lastStatus <= 0)
			lastStatus = 1;
		else
			lastStatus++;

		if (lastStatus > CounterMax)
			lastStatus = CounterMax;
	}
	else {
		// 押されていない
		if (lastStatus >= 0)
			lastStatus = -1;
		else
			lastStatus--;

		if (lastStatus < -CounterMax)
			lastStatus = -CounterMax;
	}
}

void DIKeyboardListener::Reset(){
	lastStatus = 0;
}
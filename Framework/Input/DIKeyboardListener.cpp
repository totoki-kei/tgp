#include "DIKeyboardListener.h"

DIKeyboardListener::DIKeyboardListener(int vk_key, DIKeyboardListener::KeyboardCallback f) {
	this->vk = vk_key;
	this->callback = f;
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

void DIKeyboardListener::Update(const char(&keys)[256]){
	int stat = keys[vk & 0xFF];
	if (stat & 0x80) {
		// ‰Ÿ‚³‚ê‚Ä‚¢‚é
		if (lastStatus <= 0)
			lastStatus = 1;
		else
			lastStatus++;

		if (lastStatus > CounterMax)
			lastStatus = CounterMax;
	}
	else {
		// ‰Ÿ‚³‚ê‚Ä‚¢‚È‚¢
		if (lastStatus >= 0)
			lastStatus = -1;
		else
			lastStatus--;

		if (lastStatus < -CounterMax)
			lastStatus = -CounterMax;
	}
	if (callback) callback(this);
}

void DIKeyboardListener::Reset(){
	lastStatus = 0;
}
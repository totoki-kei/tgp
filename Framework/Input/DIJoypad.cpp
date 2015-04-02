#include "DIJoypad.h"
#include "../Debug.h"

int xinputEnableCount = 0;

#pragma comment(lib, "xinput.lib")

DIJoypad::DIJoypad(int pad, bool x) : padId{ pad }, xinput{ x }
{
	if (xinput){
		if (xinputEnableCount == 0)
			XInputEnable(true);
		xinputEnableCount++;
	}
	else {
		joyGetDevCaps(padId, &joycaps, sizeof(joycaps));
	}
}


DIJoypad::~DIJoypad()
{
	if (xinput){
		xinputEnableCount--;
		if (xinputEnableCount == 0)
			XInputEnable(false);
	}
}

namespace {
	inline SHORT ConvertSHORT(DWORD value, UINT min, UINT max){
		if (value < min) {
			return SHRT_MIN;
		}
		else if (max < value) {
			return SHRT_MAX;
		}
		else {
			SHORT ret = (SHORT)( (SHRT_MAX - SHRT_MIN) * (value - min) / (max - min) + SHRT_MIN );
			if (ret == SHRT_MIN) ret += 1;
			return ret;
		}
	}

	inline BYTE ConvertBYTE(DWORD value, UINT min, UINT max){
		if (value < min) {
			return 0;
		}
		else if (max < value) {
			return UCHAR_MAX;
		}
		else {
			return (BYTE)((UCHAR_MAX) * (value - min) / (max - min));
		}
	}

	inline WORD ConvertButton(DWORD button, DWORD pov){
		WORD ret = 0;

		ret |= (button & 0x000F) << 12; // 右手親指4ボタンを想定
		ret |= (button & 0x00F0) << 4; // 左右人差し指4ボタンを想定
		ret |= (button & 0x0F00) >> 4; // メニュー系ボタンを想定

		// ボタン番号でアクセスできるようにマッピングする
		//ret |= (button & 0x0FFF) << 4;

		const int g = 36000 / 8;
		const int w = g / 2;

		signed short spov = (signed short)pov;

		if (spov != -1){
			if (g * 0 - w <= spov && spov < g * 0 + w) ret |= (XINPUT_GAMEPAD_DPAD_UP);
			if (g * 1 - w <= spov && spov < g * 1 + w) ret |= (XINPUT_GAMEPAD_DPAD_UP | XINPUT_GAMEPAD_DPAD_RIGHT);
			if (g * 2 - w <= spov && spov < g * 2 + w) ret |= (XINPUT_GAMEPAD_DPAD_RIGHT);
			if (g * 3 - w <= spov && spov < g * 3 + w) ret |= (XINPUT_GAMEPAD_DPAD_RIGHT | XINPUT_GAMEPAD_DPAD_DOWN);
			if (g * 4 - w <= spov && spov < g * 4 + w) ret |= (XINPUT_GAMEPAD_DPAD_DOWN);
			if (g * 5 - w <= spov && spov < g * 5 + w) ret |= (XINPUT_GAMEPAD_DPAD_DOWN | XINPUT_GAMEPAD_DPAD_LEFT);
			if (g * 6 - w <= spov && spov < g * 6 + w) ret |= (XINPUT_GAMEPAD_DPAD_LEFT);
			if (g * 7 - w <= spov && spov < g * 7 + w) ret |= (XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_UP);
			if (g * 8 - w <= spov && spov < g * 8 + w) ret |= (XINPUT_GAMEPAD_DPAD_UP);
		}

		return ret;
	}
}

bool DIJoypad::Update(XINPUT_STATE& state){
	if (xinput){
		switch (DWORD result = XInputGetState(padId, &state)) {
			case ERROR_SUCCESS:
				// succeeded.
				break;
			case ERROR_DEVICE_NOT_CONNECTED:
				// not plugged
				return false;
			default:
				// other error
				//LOG_DBG("XInput情報の更新に失敗しました。 リターンコード：%d\n", result);
				return false;
		}
	}
	else {
		JOYINFOEX joyinfo;
		joyinfo.dwSize = sizeof JOYINFOEX;
		joyinfo.dwFlags = JOY_RETURNALL;
		switch (MMRESULT result = joyGetPosEx(padId, &joyinfo)) {
			case JOYERR_NOERROR:
				// succeeded.
				break;
			case JOYERR_UNPLUGGED:
				// not plugged
				return false;
			default:
				// other error
				//LOG_DBG("ジョイパッド情報の更新に失敗しました。 リターンコード：%d\n", result);
				return false;
		}

		state.Gamepad.wButtons = ConvertButton(joyinfo.dwButtons, joyinfo.dwPOV);
		state.Gamepad.sThumbLX = ConvertSHORT(joyinfo.dwXpos, joycaps.wXmin, joycaps.wXmax);
		state.Gamepad.sThumbLY = -ConvertSHORT(joyinfo.dwYpos, joycaps.wYmin, joycaps.wYmax); // Yは上下逆
		state.Gamepad.sThumbRX = ConvertSHORT(joyinfo.dwZpos, joycaps.wZmin, joycaps.wRmax);
		state.Gamepad.sThumbRY = -ConvertSHORT(joyinfo.dwRpos, joycaps.wRmin, joycaps.wZmax); // Yは上下逆
		state.Gamepad.bLeftTrigger = ConvertBYTE(joyinfo.dwUpos, joycaps.wUmin, joycaps.wUmax);
		state.Gamepad.bRightTrigger = ConvertBYTE(joyinfo.dwVpos, joycaps.wVmin, joycaps.wVmax);
		
		state.dwPacketNumber++;
	}

	return true;
}

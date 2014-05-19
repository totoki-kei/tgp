#pragma once

#include "../WinAPI.h"
#include <functional>

class DIKeyboardListener;


/*
 * �L�[�{�[�h���͊Ď���
 */
class DIKeyboardListener {

public:
	static const int CounterMax = 999;
private:

	// �Ď��Ώۂ̃L�[(VK_***)
	int vk;
	// �X�e�[�^�X
	//  0�F��������(��x��Update����Ă��Ȃ�)
	//  1�ȏ�F�����ꑱ���Ă���t���[����(CounterMax���ő�)
	// -1�ȉ��F�����ꑱ���Ă���t���[����(-CounterMax���ŏ�)
	int lastStatus;
public:
	DIKeyboardListener(int vk_key);
//	virtual ~DIKeyboardListener();

	int GetLastStatus();
	__declspec(property(get = GetLastStatus))
		int $LastStatus;


	bool IsKeyDown();
	bool IsKeyUp();

	void Update(const BYTE(&keys)[256]);
	void Reset();

	int GetKey() { return vk; }
	void SetKey(int vk_key){ vk = vk_key; }
	__declspec(property(get = GetKey, put = SetKey))
		int $Key;


};

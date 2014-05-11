#pragma once

#include "../WinAPI.h"
#include <functional>

class DIKeyboardListener;


/*
 * キーボード入力監視の
 */
class DIKeyboardListener {

public:
	static const int CounterMax = 999;
	typedef std::function<void(DIKeyboardListener*)> KeyboardCallback;
private:

	// 監視対象のキー(VK_***)
	int vk;
	// コールバック
	KeyboardCallback callback;
	// ステータス
	//  0：未初期化(一度もUpdateされていない)
	//  1以上：押され続けているフレーム数(CounterMaxが最大)
	// -1以下：離され続けているフレーム数(-CounterMaxが最小)
	int lastStatus;
public:
	DIKeyboardListener(int vk_key, KeyboardCallback f = []{});
//	virtual ~DIKeyboardListener();

	int GetLastStatus();
	__declspec(property(get = GetLastStatus))
		int $LastStatus;


	bool IsKeyDown();
	bool IsKeyUp();

	void Update(const char(&keys)[256]);
	void Reset();

	int GetKey() { return vk; }
	void SetKey(int vk_key){ vk = vk_key; }
	__declspec(property(get = GetKey, put = SetKey))
		int $Key;


};


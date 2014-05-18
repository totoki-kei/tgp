#include "DIDispatcher.h"

#include "../WinAPI.h"

DIDispatcher::DIDispatcher(GameWindow* window) : historyIndex{ 0 }, noHistory{ true }
{
	wnd = window;

	wnd->SetMessageHandler(WM_MOUSEMOVE, [this](UINT, WPARAM wp, LPARAM lp){
		POINT p;
		p.x = GET_X_LPARAM(lp);
		p.y = GET_Y_LPARAM(lp);
		//ScreenToClient(wnd->GetWindowHandle(), &p);

		history[historyIndex].x = p.x;
		history[historyIndex].y = p.y;
		history[historyIndex].timestamp = GetTickCount();
		history[historyIndex + 4].x = p.x;
		history[historyIndex + 4].y = p.y;
		history[historyIndex + 4].timestamp = GetTickCount();

		return 0;
	});
}


DIDispatcher::~DIDispatcher()
{
}

std::weak_ptr<DIMouseListener> DIDispatcher::AddMouseListener(DIMouseListenType type, const DIRange& x, const DIRange& y){
	auto p = std::shared_ptr < DIMouseListener>(new DIMouseListener(type, x, y));
	mouseListener.push_back(p);
	return std::weak_ptr<DIMouseListener>{ p };
}

std::weak_ptr<DIKeyboardListener> DIDispatcher::AddKeyboardListener(int vk_key){
	auto p = std::shared_ptr < DIKeyboardListener>(new DIKeyboardListener(vk_key));
	kbdListener.push_back(p);
	return std::weak_ptr<DIKeyboardListener>{ p };
}


void DIDispatcher::Update(){
	if (mouseListener.size() > 0){
		// マウス情報の取得
		if (noHistory){
			// まだ履歴が十分でない場合は
			// 今のマウス情報を全体に展開する
			for (int i = 0; i < 8; i++){
				history[i] = history[historyIndex];
				// タイムスタンプをずらす
				history[i].timestamp += historyIndex - i;
			}
			noHistory = false;
		}

		if (history[historyIndex].timestamp == history[(historyIndex + 1) % 4].timestamp){
			// 間に1回もメッセージが発行されなかった場合は、今の時間を設定する
			history[historyIndex].timestamp = history[historyIndex].timestamp = GetTickCount();
		}

		for (auto& m : mouseListener){
			m->Update(history + historyIndex);
		}
		int newIndex = (historyIndex + 3) % 4;
		history[newIndex] = history[newIndex + 4] = history[historyIndex];
		historyIndex = newIndex;
	}

	if (kbdListener.size() > 0){
		GetKeyboardState(keyboards);

		for (auto& k : kbdListener){
			k->Update(keyboards);
		}
	}
}
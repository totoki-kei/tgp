#pragma once

class DIDispatcher;

#include <vector>
#include <memory>

#include "../GameWindow.h"
#include "DICommon.h"
#include "DIKeyboardListener.h"
#include "DIMouseListener.h"


class DIDispatcher
{
	std::vector< std::shared_ptr<DIMouseListener> > mouseListener;
	std::vector< std::shared_ptr<DIKeyboardListener> > kbdListener;

	GameWindow* wnd;

	BYTE keyboards[256];
	DIMousePointHistory history[8];
	int historyIndex;
	bool noHistory;
public:
	DIDispatcher(GameWindow* window);
	~DIDispatcher();

	std::weak_ptr<DIMouseListener> AddMouseListener(DIMouseListenType type, const DIRange& x, const DIRange& y);
	std::weak_ptr<DIKeyboardListener> AddKeyboardListener(int vk_key);

	void Update();
};


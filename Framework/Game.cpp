#include "Game.h"
#include "Debug.h"

#include <thread>

Game::Game(void)
	: exitLoop{ false },
	ticks{ -1 }, 
	updatedFlag{ false }, 
	asyncDraw{ true } {}

Game::~Game(void){}


int Game::Run(void){
	exitLoop = false;
	ticks = 0;

	if (asyncDraw) {

		std::thread drawThread{ [&]() {
			while (!exitLoop) {
				try {
					if (this->updatedFlag) {
						updatedFlag = false;
						Draw();
					}
				}
				catch (...) {
					LOG_ERR("draw exception\n", );
				}
			}
		} };

		while (!exitLoop) {
			try {
				Update();
				updatedFlag = true;
			}
			catch (...) {
				LOG_ERR("update exception\n", );
			}

			ticks++;
		}

		drawThread.join();
	}
	else {
		while (!exitLoop) {
			try {
				Update();
			}
			catch (...) {
				LOG_ERR("update exception\n", );
			}

			ticks++;

			try {
				Draw();
			}
			catch (...) {
				LOG_ERR("draw exception\n", );
			}
		}
	}


	return 0;
}

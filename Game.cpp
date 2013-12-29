#include "Game.h"
#include "Debug.h"

Game::Game(void){ exitLoop = false; ticks = -1; }

Game::~Game(void){}


int Game::Run(void){
	exitLoop = false;
	ticks = 0;
	frameskip = 0;

	while( !exitLoop ) {
		Update();
		
		if( frameskip ){
			frameskip--;
		}
		else {
			Draw();
		}
		
		ticks++;
	}
	
	return 0;
}

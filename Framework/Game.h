#pragma once

#include <atomic>

class Game
{
protected:
	bool exitLoop;
	int ticks;
	bool updatedFlag;
	bool asyncDraw;

public:
	Game();
	virtual ~Game(void);

	int Run();

	virtual int Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

Game* GenerateGame();

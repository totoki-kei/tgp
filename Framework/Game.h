#pragma once

class Game
{
protected:
	bool exitLoop;
	int ticks;
	int frameskip;

public:
	Game();
	virtual ~Game(void);

	int Run();

	virtual int Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

Game* GenerateGame();

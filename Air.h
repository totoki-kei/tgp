#pragma once

const int CELL_COUNT_X = 320;
const int CELL_COUNT_Y = 240;
const int CELL_COUNT_SLOT = 2;

struct Cell {
	double pressure;
	double vx, vy;
};

class Air
{
	Cell cells[CELL_COUNT_SLOT][(CELL_COUNT_X + 2) * (CELL_COUNT_Y + 2)];

	Cell* at(int slot, int x, int y);
	void update(Cell* outcell, const Cell* center, const Cell* upper, const Cell* lower);
public:
	Air(void);
	~Air(void);
};


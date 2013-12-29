#include "Air.h"

#include <math.h>

Cell* Air::at(int slot, int x, int y){
	return cells[slot] + ((y + 1) * CELL_COUNT_Y) + (x + 1);
}

Air::Air(void)
{
	Cell initCell = {1, 0, 0};
	Cell wallCell = {-1, 0, 0};
	for(int x = -1; x < CELL_COUNT_X + 1; x++){
		for(int y = -1; y < CELL_COUNT_Y + 1; y++){
			if(y == -1 || y == CELL_COUNT_Y || x == -1 || x == CELL_COUNT_X){
				*at(0, x, y) = *at(1, x, y) = wallCell;
			}
			else {
				*at(0, x, y) = *at(1, x, y) = initCell;
			}
		}
	}
}

void Air::update(Cell* d, const Cell* c, const Cell* u, const Cell* l){
	if(c->pressure == -1) {
		*d = *c;
		return;
	}
}

Air::~Air(void)
{
}

#include "DIMouseListener.h"


DIMouseListener::DIMouseListener(DIMouseListenType t, const DIRange& x, const DIRange& y)
: type{ t }, xrange{ x }, yrange{ y }, stat{ false }, lastValueX{ 0 }, lastValueY{ 0 }
{
}


DIMouseListener::~DIMouseListener()
{
}


void DIMouseListener::Update(DIMousePointHistory points[]){

	switch (type){
	case DIMouseListenType::Position:
		// 座標
		lastValueX = points[0].x; 
		lastValueY = points[0].y; 
		break;
	case DIMouseListenType::Velocity:
		// 速度
		double delta;

		delta = (points[0].timestamp - points[1].timestamp) / 1000.0;
		lastValueX = (points[0].x - points[1].x) / delta;
		lastValueY = (points[0].y - points[1].y) / delta;
		break;
	case DIMouseListenType::Acceleration:
		//　加速度
		double delta0, delta1;
		double vx0, vx1;
		double vy0, vy1;

		delta0 = (points[0].timestamp - points[1].timestamp) / 1000.0;
		delta1 = (points[1].timestamp - points[2].timestamp) / 1000.0;

		vx0 = (points[0].x - points[1].x) / delta0;
		vy0 = (points[0].y - points[1].y) / delta0;

		vx1 = (points[1].x - points[2].x) / delta1;
		vy1 = (points[1].y - points[2].y) / delta1;

		lastValueX = (vx1 - vx0) / (delta0 + delta1) / 1000.0;
		lastValueY = (vy1 - vy0) / (delta0 + delta1) / 1000.0;
		break;
	default:
		// ???
		return;
	}

	stat = xrange.Min <= lastValueX && lastValueX <= xrange.Max
		&& yrange.Min <= lastValueY && lastValueY <= yrange.Max;
}

#include "DIMouseListener.h"


DIMouseListener::DIMouseListener(DIMouseListenType t, const DIRange& x, const DIRange& y)
: type{ t }, xrange{ x }, yrange{ y }, stat{ false }, lastValueX{ 0 }, lastValueY{ 0 }
{
}


DIMouseListener::~DIMouseListener()
{
}


void DIMouseListener::Update(DIMousePointHistory points[], HWND wnd){

	switch (type){
	case DIMouseListenType::Position:{
		// 座標
		POINT p;
		p.x = points[0].x;
		p.y = points[0].y;
		ScreenToClient(wnd, &p);
		lastValueX = p.x;
		lastValueY = p.y;
	}
		break;
	case DIMouseListenType::Velocity:{
		// 速度
		int delta;

		delta = points[0].timestamp - points[1].timestamp;
		lastValueX = (points[0].x - points[1].x) / (double)delta;
		lastValueY = (points[0].y - points[1].y) / (double)delta;
	}
		break;
	case DIMouseListenType::Acceleration:{
		//　加速度
		int delta;
		int delta0, delta1;
		double vx0, vx1;
		double vy0, vy1;

		delta0 = points[0].timestamp - points[1].timestamp;
		delta1 = points[1].timestamp - points[2].timestamp;

		vx0 = (points[0].x - points[1].x) / delta0;
		vy0 = (points[0].y - points[1].y) / delta0;

		vx1 = (points[1].x - points[2].x) / delta1;
		vy1 = (points[1].y - points[2].y) / delta1;

		lastValueX = (vx1 - vx0) / (delta0 + delta1);
		lastValueY = (vy1 - vy0) / (delta0 + delta1);
	}
		break;
	default:
		// ???
		return;
	}

	stat = xrange.Min <= lastValueX && lastValueX <= xrange.Max
		&& yrange.Min <= lastValueY && lastValueY <= yrange.Max;
}

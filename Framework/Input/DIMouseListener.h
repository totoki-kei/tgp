#pragma once

#include "../WinAPI.h"
#include <functional>

class DIMouseListener;
enum class DIMouseListenType {
	Position,
	Velocity,
	Acceleration,
};

#include "DICommon.h"

class DIMouseListener
{
	DIMouseListenType type;
	DIRange xrange, yrange;

	bool stat;
	double lastValueX, lastValueY;

public:
	DIMouseListener(DIMouseListenType type, const DIRange& x, const DIRange& y);
	~DIMouseListener();

	void Update(DIMousePointHistory points[], HWND hWnd);

	inline bool GetStatus() const { return stat; }
	inline double GetLastValueX() const { return lastValueX; }
	inline double GetLastValueY() const { return lastValueY; }

	inline DIMouseListenType GetType() const { return type; }
	inline void SetType(DIMouseListenType t) { type = t; }
	__declspec(property(get = GetType, put = SetType))
		DIMouseListenType $Type;

	inline const DIRange& GetRangeX() const { return xrange; }
	inline void SetRangeX(const DIRange& range){ xrange = range; }
	__declspec(property(get = GetRangeX, put = SetRangeX))
		const DIRange& $RangeX;

	inline const DIRange& GetRangeY() const { return yrange; }
	inline void SetRangeY(const DIRange& range){ yrange = range; }
	__declspec(property(get = GetRangeY, put = SetRangeY))
		const DIRange& $RangeY;


};


#pragma once

#include <float.h> // for DBL_MAX

struct DIRange{
	double Min, Max;

	inline DIRange(double min, double max) {
		Min = min;
		Max = max;
	}

	inline DIRange() : DIRange(-DBL_MAX, DBL_MAX){}

	template<typename T>
	bool IsInRange(const T& t){
		return Min <= t && t <= Max;
	}
};

struct DIPoint{
	double x, y;
};

struct DIMousePointHistory {
	int x, y;
	DWORD timestamp;
};

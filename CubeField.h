#pragma once

class CubeField;

#include "GameImpl.h"

class CubeField {


public:
	Models::Model *model;
	int matIndex0;
	int matIndex1;
	int matIndex2;

	CubeField();
	~CubeField();

	void Draw();

};


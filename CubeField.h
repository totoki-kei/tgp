#pragma once

class CubeField;

#include "GameImpl.h"

class CubeField {


public:
	Models::Model *model;

	CubeField();
	~CubeField();

	void Draw();

};


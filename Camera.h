#pragma once

class Camera;

#include "GameImpl.h"
#include "Misc.h"

class Camera {

public:
	Surface currentSurface;
	XMFLOAT3 cameraPos, cameraUp;
	XMFLOAT3 targetPos, targetUp;

	Camera();
	~Camera();

	void Update(bool skipAnimation = false);
	void Set(Models::SceneParameter&);

};


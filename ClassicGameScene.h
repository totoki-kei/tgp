#pragma once

class ClassicGameScene;

#include "Framework/Scene.h"

class ClassicGameScene : public Scene {
	int tick;

protected:
	Scene* UpdateScene();
	void DrawScene();

public:
	ClassicGameScene(Scene* parent);
	ClassicGameScene(Scene&&);


};

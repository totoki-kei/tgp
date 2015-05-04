#pragma once

class ClassicGameScene;

#include "Framework/Scene.h"
#include "IGameScene.h"

class ClassicGameScene : public Scene, public IGameScene {
	int tick;

protected:
	Scene* UpdateScene() override;
	void DrawScene() override;

public:
	ClassicGameScene(Scene* parent);
	ClassicGameScene(Scene&&);
	~ClassicGameScene();

	void OnGotItem(Item&) override;
	void OnDefeatEnemy(Bullet&) override;

};

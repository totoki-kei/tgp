#pragma once

class Item;
class Bullet;

class IGameScene {
public:
	virtual void OnGotItem(Item&) = 0;
	virtual void OnDefeatEnemy(Bullet&) = 0;
};

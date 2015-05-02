#pragma once

class Item;

#include "GameImpl.h"
#include "GameObject.h"
#include "Misc.h"

class Item : public GameObject {
	static Models::Model* model;
	static int activeCount;

public:
	Item();
	~Item();

	static std::vector<Item> List;
	static Item& Create();
	static Item& Create(Surface);
	static Item& Create(float x, float y, float z);
	static Item& Create(const XMFLOAT3&);

	bool enabled;
	XMFLOAT3 axis;
	int count;

private:
	static Item& CreateItemImpl();

	void Init(const XMFLOAT3&);

public:
	void Update();
	void Draw();
	void Vanish();

	static void UpdateAll();
	static void DrawAll();
	static int GetCount();
	static void Clear();

};


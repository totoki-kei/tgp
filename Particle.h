#pragma once

#include "GameImpl.h"

#include <deque>

__declspec(align(16))
struct Particle : HeapAligned<Particle> {
public:

	Particle() = default;
	Particle(XMVECTOR pos, XMVECTOR dir, XMVECTOR size, int matindex);
	~Particle();

	XMVECTOR pos, dir;
	XMVECTOR rotAxis;
	XMVECTOR size;
	
	int count;
	int mat;

	void Update();
	void Draw();

#pragma region static members
private:

public:
	static void Generate(int count, XMFLOAT3 position, XMFLOAT3 direction, XMFLOAT3 size, float diffusion, int matindex);
	static TaskResult UpdateAll();
	static TaskResult DrawAll();

#pragma endregion
};


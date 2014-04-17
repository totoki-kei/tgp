#include "Player.h"

#include "Model.h"

namespace {
	Model* model;

	D3DIndexBuffer<> *ib_sphere; // モデル用
	D3DIndexBuffer<> *ib_corner; // ワイヤーフレーム用

	void InitializeModel(D3DCore* core){
		Vertex vertex[8 + 12];

		// 頂点の初期化
		// 立方体部分
		vertex[0 + 0] = Vertex(XMFLOAT4(-1, -1, -1,  1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[0 + 1] = Vertex(XMFLOAT4(-1, -1,  1,  1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[0 + 2] = Vertex(XMFLOAT4(-1,  1, -1,  1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[0 + 3] = Vertex(XMFLOAT4(-1,  1,  1,  1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[0 + 4] = Vertex(XMFLOAT4( 1, -1, -1,  1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[0 + 5] = Vertex(XMFLOAT4( 1, -1,  1,  1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[0 + 6] = Vertex(XMFLOAT4( 1,  1, -1,  1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[0 + 7] = Vertex(XMFLOAT4( 1,  1,  1,  1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));

		D3DIndexBuffer<>::index_t corner[] = {
			0, 1,
			0, 2,
			0, 4,
			1, 3,
			1, 5,
			2, 3,
			2, 6,
			3, 7,
			4, 5,
			4, 6,
			5, 7,
			6, 7
		};

		ib_corner = new D3DIndexBuffer<>(core, corner);

		// 切頂立方体部分
		vertex[8 + 0] = Vertex(XMFLOAT4(1, 1, 0, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[8 + 1] = Vertex(XMFLOAT4(0, 1, -1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[8 + 2] = Vertex(XMFLOAT4(1, 0, -1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[8 + 3] = Vertex(XMFLOAT4(0, 1, 1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[8 + 4] = Vertex(XMFLOAT4(1, 0, 1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[8 + 5] = Vertex(XMFLOAT4(-1, 1, 0, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[8 + 6] = Vertex(XMFLOAT4(-1, 0, 1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[8 + 7] = Vertex(XMFLOAT4(-1, 0, -1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[8 + 8] = Vertex(XMFLOAT4(0, -1, 1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[8 + 9] = Vertex(XMFLOAT4(-1, -1, 0, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[8 + 10] = Vertex(XMFLOAT4(0, -1, -1, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));
		vertex[8 + 11] = Vertex(XMFLOAT4(1, -1, 0, 1), XMFLOAT4(1, 1, 1, 1), XMFLOAT4(1, 1, 1, 1));

		D3DIndexBuffer<>::index_t sphere[] = {
			8 + 0, 8 + 1, 8 + 2,
			8 + 3, 8 + 0, 8 + 4,
			8 + 5, 8 + 3, 8 + 6,
			8 + 1, 8 + 5, 8 + 7,
			8 + 1, 8 + 0, 8 + 5,
			8 + 0, 8 + 3, 8 + 5,
			8 + 8, 8 + 3, 8 + 4,
			8 + 6, 8 + 3, 8 + 8,
			8 + 7, 8 + 5, 8 + 6,
			8 + 6, 8 + 9, 8 + 7,
			8 + 7, 8 + 10, 8 + 1,
			8 + 10, 8 + 2, 8 + 1,
			8 + 2, 8 + 4, 8 + 0,
			8 + 2, 8 + 11, 8 + 4,
			8 + 11, 8 + 8, 8 + 4,
			8 + 9, 8 + 6, 8 + 8,
			8 + 10, 8 + 7, 8 + 9,
			8 + 10, 8 + 11, 8 + 2,
			8 + 11, 8 + 10, 8 + 9,
			8 + 11, 8 + 9, 8 + 8,
		};

		ib_sphere = new D3DIndexBuffer<>(core, sphere);

		model = new Model(core, vertex, 20);


	}
}

Player::Player()
{
}

void Player::Initialize(D3DCore * core){
	InitializeModel(core);
}


Player::~Player()
{
}

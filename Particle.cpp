#include "Particle.h"

namespace {
	static Models::Model* model;
	static std::deque<Particle, new_allocator<Particle> > list;
	static std::mutex mtx;
}

Particle::Particle(XMVECTOR p, XMVECTOR v, int matindex) {
	auto g = GameImpl::GetInstance();

	pos = p;
	dir = v;
	rotAxis = XMVector3Normalize(
		XMVectorSet(g->GetRand(-1, 1), g->GetRand(-1, 1), g->GetRand(-1, 1), 1.0f)
		);
	mat = matindex;
	count = 30;
}


Particle::~Particle() {}

void Particle::Update() {
	const float k = 1 - (1 / 16.0f);

	pos = XMVectorAdd(pos, dir);
	dir = XMVectorMultiply(dir, XMVectorSet(k, k, k, 1));
	count--;
}

void Particle::Draw() {
	auto& inst = model->ReserveDraw();

	inst.World = XMMatrixTransformation(XMVectorZero(), XMQuaternionIdentity(), XMVectorSet(4, 4, 4, 1),
										XMVectorZero(), XMQuaternionRotationAxis(rotAxis, (count / 10.0f)),
										pos);
	inst.Params[0].Alpha = 10.0f;
	inst.Params[0].Blend = 1.0f;
	inst.Params[0].Index = (float)mat;
	inst.Params[0].LineWidth = count / 10.0f;

}

void Particle::Generate(int count, XMFLOAT3 position, XMFLOAT3 direction, float diffusion, int matindex) {
	XMVECTOR pos = XMVectorSet(position.x, position.y, position.z, 1.0f);
	XMVECTOR v_base = XMVectorSet(direction.x, direction.y, direction.z, 0.0f);

	auto g = GameImpl::GetInstance();

	std::lock_guard<std::mutex> lock(mtx);
	for (int i = 0; i < count; i++) {
		/*
		http://wasan.hatenablog.com/entry/2014/01/14/060137 から

		double r = cbrt(random());
		double cosTheta = random() * 2 - 1;
		double sinTheta = sqrt(1- cosTheta*cosTheta);
		double phi = random() * 2 * PI;

		double x = r * sinTheta * cos(phi);
		double y = r * sinTheta * sin(phi);
		double z = r * cosTheta;
		*/

		float r = cbrt(g->GetRand()) * diffusion;
		float cosTheta = g->GetRand(-1, 1);
		float sinTheta = sqrt(1 - cosTheta * cosTheta);
		float phi = g->GetRand(0, XM_2PI);

		XMVECTOR v_dif = XMVectorSet(
			r * sinTheta * cos(phi),
			r * sinTheta * sin(phi),
			r * cosTheta,
			0.0f);

		int mat = (matindex == -1 ? (int)g->GetRand(0, 8) : matindex);

		list.emplace_back(pos, XMVectorAdd(v_base, v_dif), mat);
	}
}

TaskResult Particle::UpdateAll() {
	std::lock_guard<std::mutex> lock(mtx);

	// 消えているパーティクルを先に削除
	while (list.size() > 0 && list.front().count <= 0) {
		list.pop_front();
	}

	// 全パーティクルを更新
	for (auto& p : list) {
		p.Update();
	}

	return TR_KEEP;
}

TaskResult Particle::DrawAll() {
	static Initializer initModel([]() {
		model = g_ModelBank->Get(_T("Content\\Model\\r01.txt"), false, true);
	});

	std::lock_guard<std::mutex> lock(mtx);

	if (!model || list.size() == 0) return TR_KEEP;

	// 全パーティクルを描画後、すぐにFlushする
	for (auto& p : list) {
		p.Draw();
	}
	model->Flush();

	return TR_KEEP;
}

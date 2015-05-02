#include "Bullet.h"
#include "Particle.h"

std::list<Bullet> Bullet::List, Bullet::Pool;
Models::Model* Bullet::model;

void Bullet::StaticInit() {
}

Bullet::Bullet() {
	if (!model) {
		model = g_ModelBank->Get(_T("Content\\Model\\Bullet.txt"), true, false);
	}
	enabled = false;
	count = 0;
	materialId = 0;
}

Bullet::~Bullet() {}

Bullet& Bullet::Shoot(Surface suf, int matid, XMFLOAT3 pos, XMFLOAT3 vel) {
	if (Pool.empty()) {
		Pool.resize(32);
	}
	List.splice(List.end(), std::move(Pool), Pool.begin());
	Bullet& bullet = List.back();

	bullet.enabled = true;
	bullet.pos = pos;
	bullet.vel = vel;
	bullet.suf = suf;
	bullet.count = 0;

	return bullet;
}

void Bullet::Update() {
	// TODO: 追加のアクションを行う場合はここに記述する。
	
	if (MoveOnSurface()) UpdateRotMatrix();
	count++;
}

void Bullet::Draw() {
	auto& inst = model->ReserveDraw();

	XMMATRIX rotmat = XMLoadFloat4x4(&rotMatrix);

	inst.World = XMMatrixScaling(1 / 2000.0f, 1 / 2000.0f, 1 / 2000.0f)
		* XMMatrixRotationZ(count / 16.0f)
		* rotmat
		* XMMatrixTranslation(pos.x, pos.y, pos.z);

	inst.Params[0].LineWidth /= 100.0f;
	inst.Params[1].LineWidth /= 100.0f;
	inst.Params[2].LineWidth /= 100.0f;
	inst.Params[3].LineWidth /= 100.0f;
	// TODO: ここに色やアルファ値を変更するコードを書く
	//inst.Params[0].Index = ...
}


void Bullet::Vanish() {
	enabled = false;
	Particle::Generate(32, this->pos, this->vel, 3.0f, materialId);
}

void Bullet::UpdateRotMatrix() {
	XMVECTOR dir = XMVector3Normalize(XMLoadFloat3(&vel));
	XMVECTOR front = XMVectorSet(0, 0, 1, 0);
	XMVECTOR cross = XMVector3Normalize(XMVector3Cross(front, dir));
	float dot = XMVectorGetX(XMVector3Dot(front, dir));

	if (XMVectorGetX(XMVector3LengthSq(cross)) < 0.5f) {
		// 正規化した外積ベクトルのサイズが1未満 => dirとfrontが平行

		if (vel.z > 0.0f) {
			XMStoreFloat4x4(&rotMatrix, XMMatrixRotationY(XM_PI));
		}
		else {
			// 備考：length(vel) == 0 の時はここに来る。
			XMStoreFloat4x4(&rotMatrix, XMMatrixIdentity());
		}
	}
	else {
		float sin_half = sqrt((1 - dot) / 2.0f);
		float cos_half = sqrt((1 + dot) / 2.0f);
		auto q = XMVectorSet(XMVectorGetX(cross) * sin_half,
							 XMVectorGetY(cross) * sin_half,
							 XMVectorGetZ(cross) * sin_half,
							 cos_half);

		XMStoreFloat4x4(&rotMatrix, XMMatrixRotationQuaternion(q));
	}
}

void Bullet::UpdateAll() {
	activeCount = 0;
	for (auto& bullet : List) {
		if (!bullet.enabled) continue;
		bullet.Update();
		activeCount++;
	}
}

void Bullet::DrawAll() {
	for (auto& bullet : List) {
		if (!bullet.enabled) continue;
		bullet.Draw();
	}
	model->Flush();
}

void Bullet::SweepToPool() {
	auto it = List.begin();

	while (it != List.end()) {
		auto next = std::next(it);
		if (!it->enabled) {
			Pool.splice(Pool.end(), std::move(List), it);
		}
		it = next;
	}
}

int Bullet::GetCount() {
	return activeCount;
}

int Bullet::activeCount;


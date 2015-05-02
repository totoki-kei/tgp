#include "Bullet.h"
#include "Particle.h"

std::list<Bullet> Bullet::List, Bullet::Pool;
Models::Model* Bullet::model;

void Bullet::StaticInit() {
}

void Bullet::NullPattern(Bullet::PatternCoroutine::Yielder& yield) {
	while (yield());
}

Bullet::Bullet() : coro(NullPattern) {
	if (!model) {
		model = g_ModelBank->Get(_T("Content\\Model\\Bullet.txt"), true, false);
	}
	enabled = false;
	count = 0;
	materialId = 0;
}

Bullet::~Bullet() {}

Bullet& Bullet::Shoot(Surface suf, int matid, XMFLOAT3 pos, XMFLOAT3 vel, Pattern&& pattern) {
	if (Pool.empty()) {
		Pool.resize(32);
	}
	List.splice(List.end(), std::move(Pool), Pool.begin());
	Bullet& bullet = List.back();

	bullet.materialId = matid;
	bullet.enabled = true;
	bullet.pos = pos;
	bullet.vel = vel;
	bullet.suf = suf;
	bullet.count = 0;
	bullet.scoreRatio = 1.0f;

	bullet.coro.Reset(std::move(pattern));

	return bullet;
}

void Bullet::Update() {
	bool e = this->enabled;
	this->enabled = coro && coro(this);
	
	if (lastRotated = !!MoveOnSurface()) UpdateRotMatrix();
	count++;
}

void Bullet::Draw() {
	auto& inst = model->ReserveDraw();

	XMMATRIX rotmat = XMLoadFloat4x4(&rotMatrix);

	inst.World = XMMatrixScaling(1 / 2000.0f, 1 / 2000.0f, 1 / 2000.0f)
		* XMMatrixRotationZ(count / 16.0f)
		* rotmat
		* XMMatrixTranslation(pos.x, pos.y, pos.z);

	//inst.Params[0].LineWidth /= 100.0f;
	//inst.Params[1].LineWidth /= 100.0f;
	//inst.Params[2].LineWidth /= 100.0f;
	//inst.Params[3].LineWidth /= 100.0f;
	inst.Params[1].Index = static_cast<float>(this->materialId);
	// TODO: ここに色やアルファ値を変更するコードを書く
	//inst.Params[0].Index = ...
}


void Bullet::Vanish(bool noParticle) {
	enabled = false;
	if (!noParticle) {
		auto particleSpeed = XMFLOAT3{ 
			this->vel.x * 2,
			this->vel.y * 2, 
			this->vel.z * 2 
		};
		Particle::Generate(32, this->pos, particleSpeed, XMFLOAT3{ 1.0f / 32, 1.0f / 32, 1.0f / 32 }, 3.0f / 256, materialId);
	}
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

float Bullet::GetScoreRatio() {
	return scoreRatio;
}

void Bullet::UpdateAll() {
	activeCount = 0;
	for (auto& bullet : List) {
		if (!bullet.enabled) continue;
		bullet.Update();
		if(bullet.enabled) activeCount++;
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

int Bullet::activeCount;
int Bullet::GetCount() {
	return activeCount;
}

void Bullet::Clear() {
	for (auto& bullet : List) {
		bullet.Vanish(true);
	}
	activeCount = 0;
}


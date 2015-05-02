#include "GameObject.h"
#include "Framework/Debug.h"
#include <utility>

GameObject::GameObject(const XMFLOAT3 p, const XMFLOAT3 v)
	: pos{ p }
	, vel{ v }
	, suf{ GetSurfaceFromVector(XMLoadFloat3(&p)) } {}

GameObject::~GameObject() {}
#if 0
bool GameObject::Turn(float& a, float& b, float& c, float& va, float& vb, float& vc) {
	if (a == 1.0f) {
		if (b > 1.0f || (b == 1.0f && vb > 0)) {
			a -= b - 1.0f;
			b = 1.0f;
			auto tmp = va;
			va = -vb;
			vb = tmp;
			return true;
		}
		else if (b < -1.0f || (b == -1.0f && vb < 0)) {
			a += b + 1.0f;
			b = -1.0f;
			auto tmp = va;
			va = vb;
			vb = tmp;
			return true;
		}
		else if (c > 1.0f || (c == 1.0f && vc > 0)) {
			a -= c - 1.0f;
			c = 1.0f;
			auto tmp = va;
			va = -vc;
			vc = tmp;
			return true;
		}
		else if (c < -1.0f || (c == -1.0f && vb < 0)) {
			a += c + 1.0f;
			c = -1.0f;
			auto tmp = va;
			va = vc;
			vc = tmp;
			return true;
		}
	}
	else if (a == -1.0f) {
		if (b > 1.0f || (b == 1.0f && vb > 0)) {
			a += b - 1.0f;
			b = 1.0f;
			auto tmp = va;
			va = vb;
			vb = tmp;
			return true;
		}
		else if (b < -1.0f || (b == -1.0f && vb < 0)) {
			a -= b + 1.0f;
			b = -1.0f;
			auto tmp = va;
			va = -vb;
			vb = tmp;
			return true;
		}
		else if (c > 1.0f || (c == 1.0f && vc > 0)) {
			a += c - 1.0f;
			c = 1.0f;
			auto tmp = va;
			va = vc;
			vc = tmp;
			return true;
		}
		else if (c < -1.0f || (c == -1.0f && vb < 0)) {
			a -= c + 1.0f;
			c = -1.0f;
			auto tmp = va;
			va = -vc;
			vc = tmp;
			return true;
		}
	}
	else {
		LOG_ERR("bad position (%f, %f, %f)\n", a, b, c);
	}
	return false;
}

bool GameObject::CheckTurn() {
	bool ret = false;
	for (int i = 0; i < 3; i++) {
		bool turned = false;
		switch (GetSurface()) {
			case SURF_XPLUS:
			case SURF_XMINUS:
				turned = Turn(pos.x, pos.y, pos.z, vel.x, vel.y, vel.z);
				break;
			case SURF_YPLUS:
			case SURF_YMINUS:
				turned = Turn(pos.y, pos.z, pos.x, vel.y, vel.z, vel.x);
				break;
			case SURF_ZPLUS:
			case SURF_ZMINUS:
				turned = Turn(pos.z, pos.x, pos.y, vel.z, vel.x, vel.y);
				break;
			default:
				break;
		}
		//turned =
		//    makeTurn(ref X, ref Y, ref Z, ref VX, ref VY, ref VZ) || 
		//    makeTurn(ref Y, ref Z, ref X, ref VY, ref VZ, ref VX) || 
		//    makeTurn(ref Z, ref X, ref Y, ref VZ, ref VX, ref VY) ;
		ret |= turned;
		if (!turned) break;
	}

	return ret;

}
#endif

int GameObject::MoveOnSurface(XMVECTOR* outQuaternion, int maxTurn, float* refDelta) {
	float* poslist[] = { &pos.x, &pos.y, &pos.z };

	float delta = refDelta ? *refDelta : 0;

	Surface currentSurface = GetSurface();
	int turnCount = 0;

	if (outQuaternion) *outQuaternion = XMQuaternionIdentity();

	while (delta <= 1) {

		// 最初に稜線を跨ぐタイミングを導出する

		// 初期値 : 残り猶予時間
		float eps = 1 - delta;

		// 初期epsが非常に小さい場合、これ以上は計算誤差でつぶれる可能性があるので
		// 処理を打ち切る。
		if (eps < 1.0f / 1024) break;

		Surface nextSurface = SURF_NONE;
		auto calceps = [&](const float p, const float v, const Surface s) {
			/*
			|pos + eps * vel| = 1
			eps = (+-1 - pos) / vel
			*/
			if (v != 0.0f) {
				// 正方向の辺に衝突するまでの時間
				float dp = (+1 - p) / v;
				// 負方向の辺に衝突するまでの時間
				float dn = (-1 - p) / v;

				float delta = 10000.0f; // 大きい値
				Surface ss = SURF_NONE;

				if (0 < dp) {
					// 正方向への移動
					delta = dp;
					ss = s;
				}
				else if (0 < dn) {
					// 負方向への移動
					delta = dn;
					ss = (Surface)(s | 0x01);
				}
				else if (dp == 0 && v > 0 && (currentSurface >> 1) != (s >> 1)) {
					// 正方向の稜線上にいる時の、正方向への移動
					delta = 0;
					ss = s;
				}
				else if (dn == 0 && v < 0 && (currentSurface >> 1) != (s >> 1)) {
					// 負方向の稜線上にいる時の、負方向への移動
					delta = 0;
					ss = (Surface)(s | 0x01);
				}


				if (delta <= eps) {
					eps = delta;
					nextSurface = ss;
				}
			}
		};

		calceps(pos.x, vel.x, SURF_XPLUS);
		calceps(pos.y, vel.y, SURF_YPLUS);
		calceps(pos.z, vel.z, SURF_ZPLUS);

		// 衝突するまで、または時間いっぱいまで移動
		pos.x += vel.x * eps;
		pos.y += vel.y * eps;
		pos.z += vel.z * eps;

		// 稜線に衝突した時には、次にどのSurfaceに移るかがここに入っている
		if (nextSurface != SURF_NONE) {
			if ((nextSurface >> 1) != (currentSurface >> 1)) {
				int epsIndex = (nextSurface >> 1);

				// 突き抜けた方向の座標は、+-1.0 に補正する
				*poslist[epsIndex] = round(*poslist[epsIndex]);

				// 突き抜けた方向への速度ベクトルを折り曲げる

				// 折り曲げ方向の回転軸の導出
				XMVECTOR rotaxis = XMVector3Cross(GetSurfaceVector(nextSurface), GetSurfaceVector(currentSurface));
				// クォータニオン導出
				XMVECTOR q = XMQuaternionRotationAxis(rotaxis, -XM_PIDIV2);

				// クォータニオンを使用して速度ベクトルを回転
				XMVECTOR p = XMLoadFloat3(&vel);
				p = XMVector3Rotate(p, q);
				XMStoreFloat3(&vel, p);

				// もしクォータニオン出力ポインタが設定されていたらそこに合成していく
				if (outQuaternion) *outQuaternion = XMQuaternionMultiply(*outQuaternion, q);

				// 折り返し回数を増加
				turnCount++;
				// 現在のSurfaceの更新
				currentSurface = nextSurface;
			}
		}
		
		// 時間を進める
		delta += eps;

		if (maxTurn && turnCount >= maxTurn) {
			break;
		}
	}

	if (refDelta) *refDelta = delta;
	suf = currentSurface;

	return turnCount;

}

Surface GameObject::GetSurface() const {
	//if (pos.x >= 1.0f) return  SURF_XPLUS;
	//if (pos.y >= 1.0f) return  SURF_YPLUS;
	//if (pos.z >= 1.0f) return  SURF_ZPLUS;
	//if (pos.x <= -1.0f) return SURF_XMINUS;
	//if (pos.y <= -1.0f) return SURF_YMINUS;
	//if (pos.z <= -1.0f) return SURF_ZMINUS;
	//return SURF_NONE;
	return suf;
}

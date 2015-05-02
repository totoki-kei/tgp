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

		// �ŏ��ɗŐ����ׂ��^�C�~���O�𓱏o����

		// �����l : �c��P�\����
		float eps = 1 - delta;

		// ����eps�����ɏ������ꍇ�A����ȏ�͌v�Z�덷�łԂ��\��������̂�
		// ������ł��؂�B
		if (eps < 1.0f / 1024) break;

		Surface nextSurface = SURF_NONE;
		auto calceps = [&](const float p, const float v, const Surface s) {
			/*
			|pos + eps * vel| = 1
			eps = (+-1 - pos) / vel
			*/
			if (v != 0.0f) {
				// �������̕ӂɏՓ˂���܂ł̎���
				float dp = (+1 - p) / v;
				// �������̕ӂɏՓ˂���܂ł̎���
				float dn = (-1 - p) / v;

				float delta = 10000.0f; // �傫���l
				Surface ss = SURF_NONE;

				if (0 < dp) {
					// �������ւ̈ړ�
					delta = dp;
					ss = s;
				}
				else if (0 < dn) {
					// �������ւ̈ړ�
					delta = dn;
					ss = (Surface)(s | 0x01);
				}
				else if (dp == 0 && v > 0 && (currentSurface >> 1) != (s >> 1)) {
					// �������̗Ő���ɂ��鎞�́A�������ւ̈ړ�
					delta = 0;
					ss = s;
				}
				else if (dn == 0 && v < 0 && (currentSurface >> 1) != (s >> 1)) {
					// �������̗Ő���ɂ��鎞�́A�������ւ̈ړ�
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

		// �Փ˂���܂ŁA�܂��͎��Ԃ����ς��܂ňړ�
		pos.x += vel.x * eps;
		pos.y += vel.y * eps;
		pos.z += vel.z * eps;

		// �Ő��ɏՓ˂������ɂ́A���ɂǂ�Surface�Ɉڂ邩�������ɓ����Ă���
		if (nextSurface != SURF_NONE) {
			if ((nextSurface >> 1) != (currentSurface >> 1)) {
				int epsIndex = (nextSurface >> 1);

				// �˂������������̍��W�́A+-1.0 �ɕ␳����
				*poslist[epsIndex] = round(*poslist[epsIndex]);

				// �˂������������ւ̑��x�x�N�g����܂�Ȃ���

				// �܂�Ȃ������̉�]���̓��o
				XMVECTOR rotaxis = XMVector3Cross(GetSurfaceVector(nextSurface), GetSurfaceVector(currentSurface));
				// �N�H�[�^�j�I�����o
				XMVECTOR q = XMQuaternionRotationAxis(rotaxis, -XM_PIDIV2);

				// �N�H�[�^�j�I�����g�p���đ��x�x�N�g������]
				XMVECTOR p = XMLoadFloat3(&vel);
				p = XMVector3Rotate(p, q);
				XMStoreFloat3(&vel, p);

				// �����N�H�[�^�j�I���o�̓|�C���^���ݒ肳��Ă����炻���ɍ������Ă���
				if (outQuaternion) *outQuaternion = XMQuaternionMultiply(*outQuaternion, q);

				// �܂�Ԃ��񐔂𑝉�
				turnCount++;
				// ���݂�Surface�̍X�V
				currentSurface = nextSurface;
			}
		}
		
		// ���Ԃ�i�߂�
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

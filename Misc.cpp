#include "Misc.h"

#include "GameImpl.h"

XMFLOAT3 GetRandomPointOnSurface(Surface s) {
	float rnd_a = GameImpl::GetInstance()->GetRand(-0.95f, 0.95f);
	float rnd_b = GameImpl::GetInstance()->GetRand(-0.95f, 0.95f);

	if (s == SURF_NONE) {
		s = (Surface)(int)(GameImpl::GetInstance()->GetRand(0, 6));
	}

	switch (s) {
		case SURF_XPLUS:
			return XMFLOAT3(1, rnd_a, rnd_b);
		case SURF_YPLUS:
			return XMFLOAT3(rnd_a, 1, rnd_b);
		case SURF_ZPLUS:
			return XMFLOAT3(rnd_a, rnd_b, 1);
		case SURF_XMINUS:
			return XMFLOAT3(-1, rnd_a, rnd_b);
		case SURF_YMINUS:
			return XMFLOAT3(rnd_a, -1, rnd_b);
		case SURF_ZMINUS:
			return XMFLOAT3(rnd_a, rnd_b, -1);
		default:
			return XMFLOAT3(0, 0, 0);
	}
}

namespace {
	static XMVECTOR s_v[] = {
		XMVectorSet(1, 0, 0, 0),
		XMVectorSet(-1, 0, 0, 0),
		XMVectorSet(0, 1, 0, 0),
		XMVectorSet(0, -1, 0, 0),
		XMVectorSet(0, 0, 1, 0),
		XMVectorSet(0, 0, -1, 0),
	};
}

XMVECTOR GetSurfaceVector(const Surface s) {
	if (s == SURF_NONE) {
		return XMVectorSet(0, 0, 0, 1);
	}
	else {
		return s_v[(int)s];
	}
}

Surface GetSurfaceFromVector(const XMVECTOR v) {
	const auto x = XMVectorGetX(v);
	if (1 <= x)
		return SURF_XPLUS;
	if (x <= -1)
		return SURF_XMINUS;

	const auto y = XMVectorGetY(v);
	if (1 <= y)
		return SURF_YPLUS;
	if (y <= -1)
		return SURF_YMINUS;

	const auto z = XMVectorGetZ(v);
	if (1 <= z)
		return SURF_ZPLUS;
	if (z <= -1)
		return SURF_ZMINUS;

	return SURF_NONE;
}

Surface GetThirdSurface(const Surface a, const Surface b) {
	auto a_ = 1 << (a >> 1);
	auto b_ = 1 << (b >> 1);
	auto c_ = a_ ^ b_;
	switch (c_) {
		case 0x06: // 0b0110
			return SURF_XPLUS;
		case 0x05: // 0b0101
			return SURF_YPLUS;
		case 0x03: // 0b0011
			return SURF_ZPLUS;
		default:
			return SURF_NONE;
	}
}

XMVECTOR GetThirdSurfaceVector(const Surface a, const Surface b) {
	auto qa = GetSurfaceVector(a);
	auto qb = GetSurfaceVector(b);

	return XMVector3Cross(qa, qb);
}

void GetSharedEdge(const Surface a, const Surface b,
				   XMVECTOR& p, XMVECTOR& axis) {
	auto va = GetSurfaceVector(a);
	auto vb = GetSurfaceVector(b);

	p = XMVectorAdd(va, vb);
	axis = XMVector3Cross(va, vb);
}

int GetSurfaceConverter(Surface from, Surface to,
						XMMATRIX* outMatrixes) {
	/**
	static auto matrix = [](Surface from, Surface to) {
		XMVECTOR p, d;
		GetSharedEdge(from, to, &p, &d);
		return XMMatrixTranslationFromVector(XMVectorNegate(p))
			* XMMatrixRotationAxis(d, XM_PIDIV2)
			* XMMatrixTranslationFromVector(p);
	};

	if (outMatrixes) {
		if (from == to) {
			// 同じ平面上なので変換しない
			// (単位クォータニオンのみ)
			outMatrixes[0] = XMMatrixIdentity();
			return 1;
		}
		else if (SurfaceIsAdjacentTo(from, to)) {
			// 隣接した平面
			outMatrixes[0] = matrix(from, to);
		}
	}
	/**/

	if (from == SURF_NONE || to == SURF_NONE)
		return 0;

	if (!outMatrixes) {
		if (from == to) 
			return 1;
		else if ((from >> 1) == (to >> 1))
			return 4;
		else
			return 3;
	}

	if (from == to) {
		// 同一平面
		outMatrixes[0] = XMMatrixIdentity();
		return 1;
	}
	else if ((from >> 1) == (to >> 1)) {
		// 逆側
		XMMATRIX* m = outMatrixes;
		for (int si = 0; si < 6; si++) {
			Surface s = (Surface)si;

			if (s == from || s == to) continue;

			XMVECTOR pos1, axis1;
			GetSharedEdge(from, s, pos1, axis1);

			XMVECTOR pos2, axis2;
			GetSharedEdge(s, to, pos2, axis2);

			*m 
				= XMMatrixTranslationFromVector(XMVectorNegate(pos1))
				* XMMatrixRotationAxis(axis1, XM_PIDIV2)
				* XMMatrixTranslationFromVector(pos1)
				* XMMatrixTranslationFromVector(XMVectorNegate(pos2))
				* XMMatrixRotationAxis(axis2, XM_PIDIV2)
				* XMMatrixTranslationFromVector(pos2);

			m++;
		}

		return 4;
	}
	else {
		// 隣接面
		Surface side1 = GetThirdSurface(from, to);
		Surface side2 = (Surface)(side1 ^ 0x01);

		{
			XMVECTOR pos, axis;
			GetSharedEdge(from, to, pos, axis);

			outMatrixes[0]
				= XMMatrixTranslationFromVector(XMVectorNegate(pos))
				* XMMatrixRotationAxis(axis, XM_PIDIV2)
				* XMMatrixTranslationFromVector(pos);
		}

		{
			XMVECTOR pos1, axis1;
			GetSharedEdge(from, side1, pos1, axis1);

			XMVECTOR pos2, axis2;
			GetSharedEdge(side1, to, pos2, axis2);

			outMatrixes[1]
				= XMMatrixTranslationFromVector(XMVectorNegate(pos1))
				* XMMatrixRotationAxis(axis1, XM_PIDIV2)
				* XMMatrixTranslationFromVector(pos1)
				* XMMatrixTranslationFromVector(XMVectorNegate(pos2))
				* XMMatrixRotationAxis(axis2, XM_PIDIV2)
				* XMMatrixTranslationFromVector(pos2);
		}

		{
			XMVECTOR pos1, axis1;
			GetSharedEdge(from, side2, pos1, axis1);

			XMVECTOR pos2, axis2;
			GetSharedEdge(side2, to, pos2, axis2);

			outMatrixes[2]
				= XMMatrixTranslationFromVector(XMVectorNegate(pos1))
				* XMMatrixRotationAxis(axis1, XM_PIDIV2)
				* XMMatrixTranslationFromVector(pos1)
				* XMMatrixTranslationFromVector(XMVectorNegate(pos2))
				* XMMatrixRotationAxis(axis2, XM_PIDIV2)
				* XMMatrixTranslationFromVector(pos2);
		}

		return 3;
	}


}

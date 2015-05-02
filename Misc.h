#pragma once

#include "Framework/Graphics/D3DHeaders.h"

enum Surface {
	SURF_XPLUS,
	SURF_XMINUS,
	SURF_YPLUS,
	SURF_YMINUS,
	SURF_ZPLUS,
	SURF_ZMINUS,

	SURF_NONE = -1,
};

XMFLOAT3 GetRandomPointOnSurface(Surface s);

XMVECTOR GetSurfaceVector(const Surface s);
Surface GetSurfaceFromVector(const XMVECTOR v);

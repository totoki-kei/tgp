#pragma once
#include "Effector.h"
#include "D3DConstantBuffer.h"

namespace Sprite {
	enum GaussianEffectorDirection {
		GAUSS_HORIZONTAL,
		GAUSS_VERTICAL,
	};

	class GaussianEffector :
		public Effector {
		D3DConstantBuffer<XMUINT4> cbTextureSize;

	public:
		GaussianEffector(D3DCore *core, GaussianEffectorDirection dir);
		~GaussianEffector();

		void Apply(D3DTexture2D*, D3DSampler*);
	};
}

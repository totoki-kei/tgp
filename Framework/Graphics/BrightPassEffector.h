#pragma once
#include "Effector.h"
#include "D3DConstantBuffer.h"

namespace Sprite {
	class BrightPassEffector :
		public Effector {

	public:
		struct Parameter {
			float threshold;
			float padding_[3];
		} current_param;

	private:
		D3DConstantBuffer<Parameter> cbParameter;


	public:
		BrightPassEffector(D3DCore* core);
		BrightPassEffector(D3DCore* core, float threshold);
		~BrightPassEffector();

		void SetThreshold(float threshold);

		float GetThreshold() const;

		void Apply(D3DTexture2D *tex, D3DSampler *smp);
	};
}

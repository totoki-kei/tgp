#include "BrightPassEffector.h"

namespace Sprite {

	BrightPassEffector::BrightPassEffector(D3DCore* core) 
		: Effector{ core, _T("Content\\System\\PS_BrightPass.cso") }
		, cbParameter{ core } {
	
		SetThreshold(0.75f);
	}

	BrightPassEffector::BrightPassEffector(D3DCore* core, float th)
		: Effector{ core, _T("Content\\System\\PS_BrightPass.cso") }
		, cbParameter{ core } {

		SetThreshold(th);
	}

	void BrightPassEffector::SetThreshold(float th) {
		Parameter p = { th, NAN, NAN, NAN };
		cbParameter.Update(&p);
		current_param = p;
	}

	float BrightPassEffector::GetThreshold() const {
		return current_param.threshold;
	}


	BrightPassEffector::~BrightPassEffector() {}

	void BrightPassEffector::Apply(D3DTexture2D* tex, D3DSampler *smp) {
		shader->Apply();
		tex->Apply(Shaders::ShaderFlag::Pixel, 0);
		smp->Apply(Shaders::ShaderFlag::Pixel, 0);
		cbParameter.Apply(Shaders::ShaderFlag::Pixel, 0);
	}
}

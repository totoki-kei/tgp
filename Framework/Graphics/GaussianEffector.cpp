#include "GaussianEffector.h"

namespace Sprite {
	GaussianEffector::GaussianEffector(D3DCore *core, GaussianEffectorDirection dir) 
		: Effector{ core, (dir == GAUSS_HORIZONTAL ? _T("Content\\System\\PS_GaussianH.cso") : _T("Content\\System\\PS_GaussianV.cso")) }
		, cbTextureSize{ core } {
	}

	GaussianEffector::~GaussianEffector() {}

	void GaussianEffector::Apply(D3DTexture2D* tex, D3DSampler *smp) {
		shader->Apply();
		tex->Apply(Shaders::ShaderFlag::Pixel, 0);
		smp->Apply(Shaders::ShaderFlag::Pixel, 0);
		
		XMUINT4 txsize = { (unsigned)tex->GetWidth(), (unsigned)tex->GetHeight(), 0, 0 };
		cbTextureSize.Update(&txsize);
		cbTextureSize.Apply(Shaders::ShaderFlag::Pixel, 0);
	}
}

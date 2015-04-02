#include "Effector.h"

#include "D3DCore.h"

namespace Sprite {
	Effector::Effector()
		: shader{ nullptr } 
	{ }

	Effector::Effector(const Effector& t)
		: shader(t.shader) {}

	Effector::Effector(Effector&& t)
		: shader(std::move(t.shader)) {}

	Effector::Effector(D3DCore* core, const TCHAR* filename)
		: shader{ Shaders::Load<Shaders::PixelShader>(core, filename) }
	{
	}

	Effector::~Effector() {
		// shader will be deleted by shared_ptr
	}

}


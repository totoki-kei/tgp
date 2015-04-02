#pragma once

namespace Sprite {
	class Effector;
}

#include "D3DTexture.h"
#include "D3DSampler.h"
#include "D3DShader.h"
#include <memory>

namespace Sprite {
	class Effector
	{
	protected:
		std::shared_ptr<Shaders::PixelShader> shader;
	public:
		Effector();
		Effector(const Effector&);
		Effector(Effector&&);
		Effector(D3DCore*, const TCHAR*);
		virtual ~Effector();

		virtual void Apply(D3DTexture2D*, D3DSampler*) = 0;
	};

}

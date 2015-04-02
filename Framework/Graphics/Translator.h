#pragma once

namespace Sprite {
	class Translator;
}

#include "Sprite.h"
#include "D3DShader.h"
#include <memory>

namespace Sprite {
	class Translator {
	protected:
		std::shared_ptr<Shaders::VertexShader> shader;
	public:
		Translator();
		Translator(const Translator&);
		Translator(Translator&&);
		Translator(D3DCore*, const TCHAR*);
		virtual ~Translator();

		virtual void Apply(D3DConstantBuffer<ScreenSize>*, D3DConstantBuffer<TextureSize>*) = 0;
	};
}

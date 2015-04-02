#include "Translator.h"

namespace Sprite{
	Translator::Translator()
	: shader{ nullptr } {}

	Translator::Translator(const Translator& t)
		: shader(t.shader) {}

	Translator::Translator(Translator&& t)
		: shader(std::move(t.shader)) {}

	Translator::Translator(D3DCore* core, const TCHAR* filename)
		: shader{ Shaders::Load<Shaders::VertexShader>(core, filename) }
	{
	}

	Translator::~Translator() {
		// shader will be deleted by shared_ptr
	}
}
#pragma once

#include "Translator.h"
#include "../Utility.h"

namespace Sprite {
	class MatrixTranslator : public Translator, public HeapAligned<MatrixTranslator> {
		D3DConstantBuffer<XMMATRIX> cbMatrix;
		XMMATRIX mt;

	public:
		MatrixTranslator(D3DCore* core);
		MatrixTranslator(D3DCore* core, const XMMATRIX& matrix);
		~MatrixTranslator();

		void SetMatrix(const XMMATRIX& matrix);
		XMMATRIX GetMatrix();

		void Apply(D3DConstantBuffer<ScreenSize>*, D3DConstantBuffer<TextureSize>*);
	};
}

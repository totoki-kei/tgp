#include "MatrixTranslator.h"

namespace Sprite {
	MatrixTranslator::MatrixTranslator(D3DCore* core)
		: Translator{ core, _T("Content\\System\\VS_3DTransform.cso") }
		, cbMatrix{ core } {
		SetMatrix(::XMMatrixIdentity());
	}

	MatrixTranslator::MatrixTranslator(D3DCore* core, const XMMATRIX& m) 
		: Translator{ core, _T("Content\\System\\VS_3DTransform.cso") }
		, cbMatrix{ core } {
		
		SetMatrix(m);
	}

	MatrixTranslator::~MatrixTranslator() {
		/* nop */
	}

	void MatrixTranslator::SetMatrix(const XMMATRIX& m) {
		cbMatrix.Update(&m);
		this->mt = m;
	}

	XMMATRIX MatrixTranslator::GetMatrix() {
		return this->mt;
	}

	void MatrixTranslator::Apply(D3DConstantBuffer<ScreenSize>* sc, D3DConstantBuffer<TextureSize>* tx) {
		shader->Apply();
		cbMatrix.Apply(Shaders::ShaderFlag::Vertex, 0);
		tx->Apply(Shaders::ShaderFlag::Vertex, 1);
	}
}

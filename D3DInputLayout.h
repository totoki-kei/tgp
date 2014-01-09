#pragma once

#include <array>
#include "D3DCore.h"
#include "D3DEffect.h"
#include "Resource.h"

class D3DInputLayout : public Resource{
	D3DCore* core;
	ID3D10InputLayout* layout;
public:
	D3DInputLayout(D3DCore *core, D3D10_INPUT_ELEMENT_DESC *elements, UINT elementsNum, D3DEffect::Technique &tech, int passIndex);
	~D3DInputLayout();

	bool isDisposed();
	void Dispose();

	void Apply();
};


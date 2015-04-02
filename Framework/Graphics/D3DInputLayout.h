#pragma once

#include <array>
#include "D3DCore.h"
#include "D3DShader.h"
#include "../Resource.h"

class D3DInputLayout : public Resource{
	D3DCore* core;
	ID3D11InputLayout* layout;
public:
	D3DInputLayout(D3DCore *core, const D3D11_INPUT_ELEMENT_DESC *elements, UINT elementsNum, Shaders::VertexShader *vs);
	~D3DInputLayout();

	bool isDisposed();
	void Dispose();

	void Apply();
};


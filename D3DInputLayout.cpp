#include "D3DInputLayout.h"


D3DInputLayout::D3DInputLayout(D3DCore *core, D3D10_INPUT_ELEMENT_DESC *elements, UINT elementsNum, D3DEffect::Technique &tech, int passIndex) : core(core)
{
	int shaderBytecodeLength;
	BYTE* shaderBytecode;

	shaderBytecode = tech.GetPassInputSignature(passIndex, &shaderBytecodeLength);

	core->GetDevice()->CreateInputLayout(
		elements,
		elementsNum,
		shaderBytecode,
		shaderBytecodeLength,
		&layout);
}


D3DInputLayout::~D3DInputLayout()
{
}

bool D3DInputLayout::isDisposed(){
	return !layout;
}

void D3DInputLayout::Dispose(){
	layout->Release();
	layout = nullptr;
}

void D3DInputLayout::Apply(){
	core->GetDevice()->IASetInputLayout(layout);
}

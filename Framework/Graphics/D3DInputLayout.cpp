#include "D3DInputLayout.h"


D3DInputLayout::D3DInputLayout(D3DCore *core, D3D11_INPUT_ELEMENT_DESC *elements, UINT elementsNum, Shaders::VertexShader *vs, int passIndex) : core(core)
{
	int shaderBytecodeLength;
	const BYTE* shaderBytecode = vs->GetBytecode(&shaderBytecodeLength);

	core->GetDevice()->CreateInputLayout(
		elements,
		elementsNum,
		shaderBytecode,
		shaderBytecodeLength,
		&layout);
}


D3DInputLayout::~D3DInputLayout()
{
	if (!isDisposed()) Dispose();
}

bool D3DInputLayout::isDisposed(){
	return !layout;
}

void D3DInputLayout::Dispose(){
	layout->Release();
	layout = nullptr;
	Resource::Dispose();
}

void D3DInputLayout::Apply(){
	core->GetDeviceContext()->IASetInputLayout(layout);
}

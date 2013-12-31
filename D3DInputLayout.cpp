#include "D3DInputLayout.h"


D3DInputLayout::D3DInputLayout(D3DCore *core, D3D10_INPUT_ELEMENT_DESC *elements, UINT elementsNum, D3DEffect::Technique &tech, int pathIndex) : core(core)
{
	auto shaderBytecode = 

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

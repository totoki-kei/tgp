#include "Model.h"

Vertex::Vertex(XMFLOAT4& pos_, XMFLOAT4& color_, XMFLOAT4& emit_)
: position(pos_), color(color_), emit(emit_) { }

Vertex::Vertex(){};


D3D11_INPUT_ELEMENT_DESC* Vertex::GetInputElementDesc(){
	static D3D11_INPUT_ELEMENT_DESC desc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "EMIT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	return desc;
}

int Vertex::GetInputElementDescCount(){ return 3; }




void Model::NameToBuffer(Model* m){
	static int id = 0;
	char name[32];

	sprintf_s(name, "ModelVB_%04d", id++);
	NameToResource(m->vertexBuffer->GetRawBuffer(), name);
}

Model::Model(D3DCore* core, Vertex* vertices, int vertexCount)
{
	this->core = core;
	this->vertexBuffer = new Vertex::buffer_t(core, vertices, vertexCount);
	this->vertexCount = vertexCount;
#ifdef _DEBUG
	NameToBuffer(this);
#endif
	this->AddResource(PtrToRes(vertexBuffer));
}


Model::~Model()
{
}

void Model::Draw(){
	vertexBuffer->Apply();
	core->Draw(vertexCount, 0);
}

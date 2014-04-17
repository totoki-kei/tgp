#pragma once

#include "D3DCore.h"
#include "D3DBuffer.h"

struct Vertex {
	XMFLOAT4 position;
	XMFLOAT4 color;
	XMFLOAT4 emit;

	static D3D11_INPUT_ELEMENT_DESC* GetInputElementDesc();
	static int GetInputElementDescCount();

	Vertex(XMFLOAT4& pos, XMFLOAT4& color, XMFLOAT4& emit);
	Vertex();

	typedef D3DVertexBuffer<Vertex> buffer_t;
};


class Model : public Resource 
{
	D3DCore* core;
	Vertex::buffer_t* vertexBuffer;
	int vertexCount;

	static void NameToBuffer(Model*);

public:
	Model(D3DCore* core, Vertex* vertices, int vertexCount);
	~Model();

	void Draw();

	template <typename IndexBufferT = D3DIndexBuffer<> >
	void Draw(IndexBufferT *indices){
		if (indices) indices->Apply();
		else IndexBufferT::Unapply(core);
		Draw();
	}

};


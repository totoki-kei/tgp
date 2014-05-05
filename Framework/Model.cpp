#include "Model.h"


#include <stdio.h>

namespace Models {

#pragma region Static Fields/Functions
	D3DCore* Model::core;
	D3DInputLayout *Model::inputLayout;
	D3DRasterizer *Model::rasterizer;
	Shaders::VertexShader *Model::vsTransform;
	Shaders::GeometryShader *Model::gsEdge;
	Shaders::PixelShader *Model::psColoring;
	Shaders::ClassLinkage  *Model::classLinkage;
	Shaders::ClassInstance *Model::instNormalColor;
	Shaders::ClassInstance *Model::instEmitColor;
	Shaders::ClassInstance *Model::instLightedColor;


	D3DConstantBuffer<SceneParameter> *Model::cbScene;
	D3DConstantBuffer<ObjectParameter> *Model::cbObject;
	
	// Load‚Í•Êregion‚É‹Lq

	void Model::InitializeSharedResource(D3DCore* core){
		Model::core = core;

		classLinkage = new Shaders::ClassLinkage(core);
		core->AddResource(PtrToRes(classLinkage));

		vsTransform = Shaders::Load<Shaders::VertexShader>(core, _T("VS_Transform.cso"));
		core->AddResource(PtrToRes(vsTransform));
		gsEdge = Shaders::Load<Shaders::GeometryShader>(core, _T("GS_CreateEdge.cso"));
		core->AddResource(PtrToRes(gsEdge));
		psColoring = Shaders::Load<Shaders::PixelShader>(core, _T("PS_OutputColor.cso"), classLinkage);
		core->AddResource(PtrToRes(psColoring));
		
		instNormalColor = classLinkage->Create("NormalColor");
		instEmitColor = classLinkage->Create("EmitColor");
		instLightedColor = classLinkage->Create("LightedColor");

		inputLayout = new D3DInputLayout(core, Vertex::GetInputElementDesc(), Vertex::GetInputElementDescCount(), vsTransform, 0);
		core->AddResource(PtrToRes(inputLayout));

		D3D11_RASTERIZER_DESC rsd;
		D3DRasterizer::GetDefaultRasterizerDesc(&rsd);
		//rsd.FillMode = ...
		rasterizer = new D3DRasterizer(core);
		core->AddResource(PtrToRes(rasterizer));

		cbScene = new D3DConstantBuffer<SceneParameter>(core);
		NameToResource(cbScene->GetRawBuffer(), "Model::cbScene");
		core->AddResource(PtrToRes(cbScene));
		cbObject = new D3DConstantBuffer<ObjectParameter>(core);
		NameToResource(cbObject->GetRawBuffer(), "Model::cbObject");
		core->AddResource(PtrToRes(cbObject));
		//cbSubset = new D3DConstantBuffer<SubsetParameter>(core);
		//NameToResource(cbSubset->GetRawBuffer(), "Model::cbSubset");
		//core->AddResource(PtrToRes(cbSubset));
	}

	void Model::NameToModelResource(const char* modelType, Model* m){
		static int id = 0;
		char name[32];

		sprintf_s(name, "Model%s_%04d", modelType, id++);
		NameToResource(m->vertexBuffer->GetRawBuffer(), name);
	}

	void Model::ApplyEffect(ColoringType colortype){
		
		inputLayout->Apply();
		vsTransform->Apply();
		gsEdge->Apply();

		switch (colortype){
		case COLORING_LIGHTED:
			psColoring->Apply({ instLightedColor });
			break;
		case COLORING_EMIT:
			psColoring->Apply({ instEmitColor });
			break;
		case COLORING_NORMAL:
		default:
			psColoring->Apply({ instNormalColor });
			break;
		}

		Shaders::ShaderFlag flg = Shaders::ShaderFlag::Render;
		cbScene->Apply(flg, 0);
		cbObject->Apply(flg, 1);
		ModelSubset::ConstantBuffer::Unapply(core, flg, 2);
		//cbSubset->Apply(flg, 2);
	}

#pragma endregion

#pragma region Vertex

	Vertex::Vertex(const XMFLOAT4& pos_, const XMFLOAT4& color_, const XMFLOAT4& emit_)
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

#pragma endregion

#pragma region ModelSubset

	ModelSubset::ModelSubset(const int indexCount, const D3DIndexBuffer<>::index_t *indices, const SubsetParameter* param, bool isWireframe, ColoringType type) :
		indexBuffer(Model::core, indices, indexCount, true),
		subsetParam(Model::core)
	{
		this->isWireframe = isWireframe;
		this->coloringType = type;
		if (param){
			subsetParam.Update(param);
		}
		else {
			ConstantBuffer::data_t data;
			data.BaseColor = XMFLOAT4(0, 0, 0, 0);
			data.EdgeGradient = XMFLOAT2(0, 1);
			subsetParam.Update(&data);
		}
	}

	ModelSubset::~ModelSubset(){
		// nothing to do ...?
	}

	void ModelSubset::Draw(Model *m){
		this->subsetParam.Apply(Shaders::ShaderFlag::Render, 2);
		this->indexBuffer.Apply();
		if (this->isWireframe){
			Model::core->SetPrimitiveTopology(D3DPrimitiveTopology::LineList);
		}
		else {
			Model::core->SetPrimitiveTopology(D3DPrimitiveTopology::TriangleList);
		}
		Model::core->DrawIndexed(this->indexBuffer.GetLength(), 0, 0);
	}

#pragma endregion

#pragma region Model

	Model::Model(Vertex* vertices, int vertexCount) :
		subsets()
	{
		this->core = core;
		this->vertexBuffer = new Vertex::buffer_t(core, vertices, vertexCount);
		this->vertexCount = vertexCount;
#ifdef _DEBUG
		NameToModelResource("VB", this);
#endif
		this->AddResource(PtrToRes(vertexBuffer));
	}

	Model::~Model()
	{
	}

	void Model::Draw(){
		if (subsets.size() == 0){
			vertexBuffer->Apply();
			core->Draw(vertexCount, 0);
			return;
		}

		vertexBuffer->Apply();
		for(auto ss : subsets) {
			ApplyEffect(ss->$ColoringType);
			ss->Draw(this);
		}
	}

	void Model::DrawSubset(int index){
		if (index < 0 || subsets.size() <= index) return;

		ModelSubset &ss = *subsets[index];
		ApplyEffect(ss.$ColoringType);
		vertexBuffer->Apply();
		ss.Draw(this);
	}

#pragma endregion

}
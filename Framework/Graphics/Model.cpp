#include "Model.h"


#include <stdio.h>

namespace Models {


#pragma region Vertex

	Vertex::Vertex(const XMFLOAT4& pos_, const XMFLOAT4& color_, const XMFLOAT3& normal_)
		: position(pos_), color(color_), normal(normal_) { }

	Vertex::Vertex(){};

	const D3D11_INPUT_ELEMENT_DESC* Vertex::GetInputElementDesc(){
		static const D3D11_INPUT_ELEMENT_DESC desc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				/* -------- */
				{ "PARAM_INDEX", 0, DXGI_FORMAT_R32_UINT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		return desc;
	}

	int Vertex::GetInputElementDescCount(){ return 4; }

#pragma endregion

#pragma region Static Fields/Functions
	D3DCore* Model::core;
	D3DInputLayout *Model::inputLayout;

	D3DRasterizer *Model::rasterizer;

	Shaders::VertexShader *Model::vsTransform;
	Shaders::VertexShader *Model::vsTransformSingle;
	Shaders::GeometryShader *Model::gsLine;
	Shaders::GeometryShader *Model::gsFlat;
	Shaders::PixelShader *Model::psColoring;
	Shaders::PixelShader *Model::psNoLight;
	
	D3DBuffer::Mapper<SceneParameter> Model::mScene;
	D3DConstantBuffer<SceneParameter> *Model::cbScene;


	void Model::InitializeSharedResource(D3DCore* core){
		Model::core = core;

		vsTransform = Shaders::Load<Shaders::VertexShader>(core, _T("Content\\System\\VS_Transform.cso"));
		core->AddResource(PtrToRes(vsTransform));
		vsTransformSingle = Shaders::Load<Shaders::VertexShader>(core, _T("Content\\System\\VS_TransformSingle.cso"));
		core->AddResource(PtrToRes(vsTransformSingle));
		gsLine = Shaders::Load<Shaders::GeometryShader>(core, _T("Content\\System\\GS_WideLine.cso"));
		core->AddResource(PtrToRes(gsLine));
		gsFlat = Shaders::Load<Shaders::GeometryShader>(core, _T("Content\\System\\GS_FlatShading.cso"));
		core->AddResource(PtrToRes(gsFlat));
		psColoring = Shaders::Load<Shaders::PixelShader>(core, _T("Content\\System\\PS_LightedColor.cso"));
		core->AddResource(PtrToRes(psColoring));
		psNoLight = Shaders::Load<Shaders::PixelShader>(core, _T("Content\\System\\PS_UnlightedColor.cso"));
		core->AddResource(PtrToRes(psNoLight));
		
		inputLayout = new D3DInputLayout(core, Vertex::GetInputElementDesc(), Vertex::GetInputElementDescCount(), vsTransform);
		core->AddResource(PtrToRes(inputLayout));

		D3D11_RASTERIZER_DESC rsd;
		D3DRasterizer::GetDefaultRasterizerDesc(&rsd);
		//rsd.FillMode = ...
		rasterizer = new D3DRasterizer(core);
		core->AddResource(PtrToRes(rasterizer));

		cbScene = new D3DConstantBuffer<SceneParameter>(core);
		NameToResource(cbScene->GetRawBuffer(), "Model::cbScene");
		core->AddResource(PtrToRes(cbScene));
	}

	void Model::NameToModelResource(Model* m){
		static int id = 0;

		NameToResourceFormated(m->vertexBuffer->GetRawBuffer(), "Model%04d_vb", id);
		if (m->solidIndexBuffer) NameToResourceFormated(m->solidIndexBuffer->GetRawBuffer(), "Model%04d_ibSolid", id);
		if (m->edgeIndexBuffer) NameToResourceFormated(m->edgeIndexBuffer->GetRawBuffer(), "Model%04d_ibEdge", id);
		NameToResourceFormated(m->sbInstanceBuffer->GetRawBuffer(), "Model%04d_sbInst", id);
		NameToResourceFormated(m->cbMaterialSet->GetRawBuffer(), "Model%04d_cbMat", id);
		id++;
	}


#pragma endregion

#pragma region Model

	Model::Model(Vertex* vertices, size_t vertexCount, Vertex::ExtraInfo* extra,
				 D3DIndexBuffer<>::index_t* solidIndices, size_t solidIndicesCount,
				 D3DIndexBuffer<>::index_t* edgeIndices, size_t edgeIndicesCount, 
				 InstanceData* inst) {
		this->core = core;

		this->vertexBuffer = new Vertex::buffer_t(core, vertices, vertexCount);
		this->AddResource(PtrToRes(vertexBuffer));

		if (extra) {
			this->vertexExtraBuffer = new Vertex::buffer_extra_t(core, extra, vertexCount);
			this->AddResource(PtrToRes(vertexExtraBuffer));
		}
		else {
			this->vertexExtraBuffer = nullptr;
		}

		if (inst) {
			this->defaultInstanceParams.reset(inst);
		}

		if (solidIndices) {
			this->solidIndexBuffer = new D3DIndexBuffer<>(core, solidIndices, solidIndicesCount);
			this->AddResource(PtrToRes(solidIndexBuffer));
		}
		else {
			this->solidIndexBuffer = nullptr;
		}

		if (edgeIndices) {
			this->edgeIndexBuffer = new D3DIndexBuffer<>(core, edgeIndices, edgeIndicesCount);
			this->AddResource(PtrToRes(edgeIndexBuffer));
		}
		else {
			this->edgeIndexBuffer = nullptr;
		}

		this->sbInstanceBuffer = new D3DShaderResourceBuffer<InstanceData, InstanceCount>(core);
		this->AddResource(PtrToRes(sbInstanceBuffer));
		this->cbMaterialSet = new D3DConstantBuffer<MaterialSetParameter>(core);
		this->AddResource(PtrToRes(cbMaterialSet));

		this->solidEnabled = this->edgeEnabled = this->edgeWidthEnabled = this->flatShading = true;

#ifdef _DEBUG
		NameToModelResource(this);
#endif

		materialCount    = 0;
		instanceCount    = 0;
		pointLightCount  = 0;
	}
	
	Model::~Model()
	{
	}

	void Model::Draw(const InstanceData& inst) {
		if (instanceCount == InstanceCount) {
			// TODO: 処理を変更し、今の内容をFlushしてクリアするのでは無く
			// バッファを拡張して一括描画する量を増やす方向に変える。
			Flush();
		}
		if (!mInstanceBuffer) mInstanceBuffer.Map(sbInstanceBuffer);
		mInstanceBuffer[instanceCount++] = inst;
	}

	InstanceData& Model::ReserveDraw() {
		if (instanceCount == InstanceCount) {
			// TODO: 処理を変更し、今の内容をFlushしてクリアするのでは無く
			// バッファを拡張して一括描画する量を増やす方向に変える。
			Flush();
		}
		if (!mInstanceBuffer) mInstanceBuffer.Map(sbInstanceBuffer);
		if (defaultInstanceParams) {
			mInstanceBuffer[instanceCount] = *defaultInstanceParams;
		}
		return mInstanceBuffer[instanceCount++];
	}

	void Model::SetMaterial(int index, MaterialData& mat) {
		if (!mMaterialSet)mMaterialSet.Map(cbMaterialSet);
		mMaterialSet->Materials[index] = mat;
	}

	void Model::Flush() {
		Flush(solidEnabled, edgeEnabled);
	}

	void Model::Flush(bool solid, bool edge) {
		if (instanceCount == 0) return;

		mInstanceBuffer.Unmap();
		mScene.Unmap();
		mMaterialSet.Unmap();

		sbInstanceBuffer->Apply(Shaders::ShaderFlag::Render, 0);
		cbScene->Apply(Shaders::ShaderFlag::Render, 1);
		cbMaterialSet->Apply(Shaders::ShaderFlag::Render, 2);

		vertexBuffer->Apply(vertexExtraBuffer);
		inputLayout->Apply();



		if (instanceCount == 1) {
			vsTransformSingle->Apply(); // 単一インスタンス用シェーダを設定
		}
		else {
			vsTransform->Apply(); // マルチインスタンス用シェーダを設定
		}



		if (edge && edgeIndexBuffer) {
			if (this->edgeWidthEnabled) {
				gsLine->Apply();
			}
			else {
				Shaders::Unapply(gsLine->GetCore(), Shaders::ShaderFlag::Geometry);
			}
			psNoLight->Apply();
			edgeIndexBuffer->Apply();
			core->SetPrimitiveTopology(D3DPrimitiveTopology::LineList);
			core->DrawIndexed((int)edgeIndexBuffer->GetLength(), 0, 0, instanceCount);
		}
		if (solid && solidIndexBuffer) {
			if (flatShading) {
				gsFlat->Apply();
			}
			else {
				Shaders::Unapply(gsFlat->GetCore(), Shaders::ShaderFlag::Geometry);
			}
			psColoring->Apply();
			solidIndexBuffer->Apply();
			core->SetPrimitiveTopology(D3DPrimitiveTopology::TriangleList);
			core->DrawIndexed((int)solidIndexBuffer->GetLength(), 0, 0, instanceCount);
		}

		instanceCount = 0;
	}


#pragma endregion

}
#include "Model.h"


#include <stdio.h>

namespace Models {

#pragma region Static Fields/Functions
	D3DCore* Model::core;
	D3DInputLayout *Model::inputLayout;
	D3DRasterizer *Model::rasterizer;
	Shaders::VertexShader *Model::vsTransform;
	Shaders::GeometryShader *Model::gsEdge;
	Shaders::PixelShader *Model::psNormalColor;
	Shaders::PixelShader *Model::psEmitColor;

	D3DConstantBuffer<SceneParameter> *Model::cbScene;
	D3DConstantBuffer<ObjectParameter> *Model::cbObject;
	
	// Loadは別regionに記述

	void Model::InitializeSharedResource(D3DCore* core){
		Model::core = core;

		vsTransform = Shaders::Load<Shaders::VertexShader>(core, _T("VS_Transform.cso"));
		core->AddResource(PtrToRes(vsTransform));
		gsEdge = Shaders::Load<Shaders::GeometryShader>(core, _T("GS_CreateEdge.cso"));
		core->AddResource(PtrToRes(gsEdge));
		psNormalColor = Shaders::Load<Shaders::PixelShader>(core, _T("PS_NormalColor.cso"));
		core->AddResource(PtrToRes(psNormalColor));
		psEmitColor = Shaders::Load<Shaders::PixelShader>(core, _T("PS_EmitColor.cso"));
		core->AddResource(PtrToRes(psEmitColor));

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
		case COLORING_EMIT:
			psEmitColor->Apply();
		case COLORING_NORMAL:
		default:
			psNormalColor->Apply();
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


#pragma region Load Function

	namespace {
		int ReadParam(char* buffer, size_t bufferSize, char* &p) {
			char* start = p;
			while (iswspace(*start) && *start != '/' && *start != '\0') start++;

			char* slash = start;
			while (*slash != '/' && *slash != '\0') slash++;

			if (slash == start) {
				buffer[0] = '\0';
				p = start + 1;
				return 0;
			}

			char* end = slash - 1;
			while (iswspace(*end) && end != p) end--;

			int count = 0;
			for (auto rp = start; rp != end; rp++){
				if (count >= bufferSize - 1) break;

				buffer[count++] = *rp;
			}
			buffer[count] = '\0';
			p = slash;
			if (*slash != '/') p++;

			return count;
		}
		template<int Length>
		inline int ReadParam(char(&buffer)[Length], char* &p){
			return ReadParam(buffer, Length, p);
		}

		Vertex ReadVertex(char* p, Vertex& tpl){
			Vertex v = tpl;

			// 位置情報の読み取り
			{
				char part[128];
				char *sp = part, *ep = nullptr;
				double n;
				ReadParam(part, p);

				n = strtod(sp, &ep);
				if (sp == ep) goto POSITION_END;
				v.position.x = n;
				sp = ep;

				n = strtod(sp, &ep);
				if (sp == ep) goto POSITION_END;
				v.position.y = n;
				sp = ep;

				n = strtod(sp, &ep);
				if (sp == ep) goto POSITION_END;
				v.position.z = n;
				sp = ep;

				n = strtod(sp, &ep);
				if (sp == ep) goto POSITION_END;
				v.position.w = n;
				sp = ep;

			}
		POSITION_END:

			// 色情報１の読み取り
			{
				char part[128];
				char *sp = part, *ep = nullptr;
				double n;
				ReadParam(part, p);

				n = strtod(sp, &ep);
				if (sp == ep) goto COLOR_END;
				v.color.x = n;
				sp = ep;

				n = strtod(sp, &ep);
				if (sp == ep) goto COLOR_END;
				v.color.y = n;
				sp = ep;

				n = strtod(sp, &ep);
				if (sp == ep) goto COLOR_END;
				v.color.z = n;
				sp = ep;

				n = strtod(sp, &ep);
				if (sp == ep) goto COLOR_END;
				v.color.w = n;
				sp = ep;

			}
		COLOR_END:

			// 色情報２の読み取り
			{
				char part[128];
				char *sp = part, *ep = nullptr;
				double n;
				ReadParam(part, p);

				n = strtod(sp, &ep);
				if (sp == ep) goto EMIT_END;
				v.emit.x = n;
				sp = ep;

				n = strtod(sp, &ep);
				if (sp == ep) goto EMIT_END;
				v.emit.y = n;
				sp = ep;

				n = strtod(sp, &ep);
				if (sp == ep) goto EMIT_END;
				v.emit.z = n;
				sp = ep;

				n = strtod(sp, &ep);
				if (sp == ep) goto EMIT_END;
				v.emit.w = n;
				sp = ep;

			}
		EMIT_END:

			return v;
		}

		void ReadSubsetParam(char* p, ColoringType* color, SubsetParameter* param){

		}

		template<typename VectorT>
		void ReadIndices(char* p, VectorT &vec){
			// TODO : 実装
		}
	}

	Model* Model::Load(const TCHAR* filename) {
		FILE* fp = _tfopen(filename, _T("rt"));
		if (fp) return nullptr;

		char line[256] = { 0 };

		char* p = nullptr;

		Vertex vtemplate;
		vtemplate.position = XMFLOAT4(0, 0, 0, 1);
		vtemplate.color = XMFLOAT4(1, 1, 1, 1);
		vtemplate.emit = XMFLOAT4(0, 0, 0, 0);

		vector<Vertex> vertices;

		vector<shared_ptr<ModelSubset>> subsets;
		vector<ModelSubset::IndexBuffer::index_t> indices;
		ColoringType sscolor;
		SubsetParameter ssparam;

		while (p = fgets(line, 256, fp)) {
			// コメントか空白行であった場合はスキップ
			while (iswspace(*p)) p++;
			if (*p == '#' || *p == '\0') continue;

			// 最初のパラメータ(命令種別)を読む
			char op[4];
			ReadParam(op, p);
			switch (op[0]){
			case 'V':
				// 頂点テンプレート
				vtemplate = ReadVertex(p, vtemplate);
				break;
			case 'v':
				// 頂点
				vertices.push_back(ReadVertex(p, vtemplate));
				break;
			case 'g':
				// グループ開始
				if (indices.size() > 0){
					// 前のグループが存在したらその分をサブセットとして確定
					auto data = indices.data();
					subsets.push_back(shared_ptr<ModelSubset>(new ModelSubset(
						indices.size(),
						data,
						&ssparam,
						false,
						sscolor)));

					indices.clear();
				}
				ReadSubsetParam(p, &sscolor, &ssparam);
				break;
			case 'f':
				// 三角形
				ReadIndices(p, indices);
				break;
			}
		}

		// ループ終了後
		if (indices.size() > 0){
			// 前のグループが存在したらその分をサブセットとして確定
			auto data = indices.data();
			subsets.push_back(shared_ptr<ModelSubset>(new ModelSubset(
				indices.size(),
				data,
				&ssparam,
				false,
				sscolor)));

			indices.clear();
		}


		auto vp = vertices.data();
		Model* m = new Model(vp, vertices.size());
		std::copy(subsets.begin(), subsets.end(), std::back_inserter(m->subsets));

		fclose(fp);

		return m;
	}
#pragma endregion
}
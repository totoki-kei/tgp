#include "Sprite.h"

#include <memory>

namespace Sprite {
	TCHAR DefaultEffectorShaderName[] = _T("Content\\System\\PS_TextureOnly.cso");
	TCHAR DefaultTranslatorShaderName[] = _T("Content\\System\\VS_2DTransform.cso");

	class DefaultEffector : public Effector {
	public:
		DefaultEffector(D3DCore *core)
			: Effector{ core, DefaultEffectorShaderName } {}
		~DefaultEffector() {}

		void Apply(D3DTexture2D* tex, D3DSampler* smp){
			shader->Apply();
			tex->Apply(Shaders::ShaderFlag::Pixel, 0);
			smp->Apply(Shaders::ShaderFlag::Pixel, 0);
		}
	};

	class DefaultTranslator : public Translator {
		friend class Sprite;
	public:
		DefaultTranslator(D3DCore* core)
			: Translator{ core, DefaultTranslatorShaderName } {}
		~DefaultTranslator() {}

		void Apply(D3DConstantBuffer<ScreenSize>* sc, D3DConstantBuffer<TextureSize>* tx) override {
			shader->Apply();
			sc->Apply(Shaders::ShaderFlag::Vertex, 0);
			tx->Apply(Shaders::ShaderFlag::Vertex, 1);
		}
	};


	D3D11_INPUT_ELEMENT_DESC* Vertex::GetInputElementDesc() {
		static D3D11_INPUT_ELEMENT_DESC desc[] = {
				{ "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		return desc;
	}

	void Sprite::InitializeSharedResource(D3DCore* c){
		// 二重初期化の防止
		if (core) return;
		core = c;


		defaultTranslator = new DefaultTranslator(core);
		core->AddResource(PtrToRes(defaultTranslator));

		defaultEffector = new DefaultEffector(core);
		core->AddResource(PtrToRes(defaultEffector));

		inputLayout = new D3DInputLayout(core, Vertex::GetInputElementDesc(), 2, defaultTranslator->shader.get());
		core->AddResource(PtrToRes(inputLayout));


		cbScreenSize = new D3DConstantBuffer<ScreenSize>(core);
		core->AddResource(PtrToRes(cbScreenSize));

		ScreenSize ss;
		ss.Width = core->GetScreenWidth();
		ss.Height = core->GetScreenHeight();
		cbScreenSize->Update(&ss);


		cbTextureSize = new D3DConstantBuffer<TextureSize>(core);
		core->AddResource(PtrToRes(cbTextureSize));

		TextureSize ts;
		ts.Width = 1;
		ts.Height = 1;
		cbTextureSize->Update(&ts);

		defaultSampler = new D3DSampler(core);
		core->AddResource(PtrToRes(defaultSampler));

		D3D11_DEPTH_STENCIL_DESC dsdesc;
		D3DStencilState::GetDefaultDepthStencilDesc(&dsdesc);
		dsdesc.DepthEnable = false;
		Sprite::stencliState = new D3DStencilState(core, &dsdesc);
		core->AddResource(PtrToRes(Sprite::stencliState));
	}

	Sprite::Sprite(D3DTexture2D* tx, D3DSampler* smp, Translator* tr, Effector* ef) 
		: texture{ tx }
		, sampler{ smp }
		, translator{ tr }
		, effector{ ef }
		, vbuffer{ nullptr }
		, ibuffer{ nullptr }
	{
		if (!smp) sampler = defaultSampler;
		if (!tr) translator = defaultTranslator;
		if (!ef) effector = defaultEffector;
	}

	Sprite::~Sprite() { 
		if (vbuffer) delete vbuffer;
		if (ibuffer) delete ibuffer;
	}

	void Sprite::ReserveBuffer(int qs){
		vertices.reserve(qs * 4);
		indices.reserve(qs * 6);
	}

	void Sprite::DrawQuad(){
		float sw = (float)core->GetScreenWidth();
		float sh = (float)core->GetScreenHeight();
		float tw = (float)texture->GetWidth();
		float th = (float)texture->GetHeight();
		Vertex lt = { XMFLOAT4{ 0, 0, 0, 1 }, XMFLOAT2{ 0, 0 } };
		Vertex lb = { XMFLOAT4{ 0, sh, 0, 1 }, XMFLOAT2{ 0, th } };
		Vertex rt = { XMFLOAT4{ sw, 0, 0, 1 }, XMFLOAT2{ tw, 0 } };
		Vertex rb = { XMFLOAT4{ sw, sh, 0, 1 }, XMFLOAT2{ tw, th } };

		DrawQuad(lt, lb, rt, rb);
	}

	void Sprite::DrawQuad(const float l, const float t, const float r, const float b) {
		float tw = (float)texture->GetWidth();
		float th = (float)texture->GetHeight();
		Vertex lt = { XMFLOAT4{ l, t, 0, 1 }, XMFLOAT2{ 0, 0 } };
		Vertex lb = { XMFLOAT4{ l, b, 0, 1 }, XMFLOAT2{ 0, th } };
		Vertex rt = { XMFLOAT4{ r, t, 0, 1 }, XMFLOAT2{ tw, 0 } };
		Vertex rb = { XMFLOAT4{ r, b, 0, 1 }, XMFLOAT2{ tw, th } };
		DrawQuad(lt, lb, rt, rb);
	}

	void Sprite::DrawQuad(const XMFLOAT4& lt, const XMFLOAT4& lb, const XMFLOAT4& rt, const XMFLOAT4& rb){
		float tw = (float)texture->GetWidth();
		float th = (float)texture->GetHeight();
		Vertex v_lt = { lt, XMFLOAT2{ 0, 0 } };
		Vertex v_lb = { lb, XMFLOAT2{ 0, th } };
		Vertex v_rt = { rt, XMFLOAT2{ tw, 0 } };
		Vertex v_rb = { rb, XMFLOAT2{ tw, th } };

		DrawQuad(v_lt, v_lb, v_rt, v_rb);
	}

	void Sprite::DrawQuad(const XMFLOAT4* ps){
		float tw = (float)texture->GetWidth();
		float th = (float)texture->GetHeight();
		Vertex v_lt = { ps[0], XMFLOAT2{ 0, 0 } };
		Vertex v_lb = { ps[1], XMFLOAT2{ 0, th } };
		Vertex v_rt = { ps[2], XMFLOAT2{ tw, 0 } };
		Vertex v_rb = { ps[3], XMFLOAT2{ tw, th } };

		DrawQuad(v_lt, v_lb, v_rt, v_rb);
	}

	void Sprite::DrawQuad(const Vertex& lt, const Vertex& lb, const Vertex& rt, const Vertex& rb){
		vertices.push_back(lt);
		vertices.push_back(lb);
		vertices.push_back(rt);
		vertices.push_back(rb);

		unsigned short i = (unsigned short)vertices.size();
		indices.push_back(i - 3);
		indices.push_back(i - 2);
		indices.push_back(i - 1);
		indices.push_back(i - 2);
		indices.push_back(i - 3);
		indices.push_back(i - 4);
	}

	void Sprite::DrawQuad(const Vertex* vs){
		DrawQuad(vs[0], vs[1], vs[2], vs[3]);
	}

	void Sprite::Flush(){
		TextureSize ts = { texture->GetWidth(), texture->GetHeight() };
		cbTextureSize->Update(&ts);

		inputLayout->Apply();

		translator->Apply(cbScreenSize, cbTextureSize);
		Shaders::Unapply(core, Shaders::ShaderFlag::Geometry);
		effector->Apply(texture, sampler);

		if (!vbuffer){
			// バッファの作成
			vbuffer = new D3DVertexBuffer<Vertex>(core, vertices.data(), vertices.size(), true);
		}
		else {
			if (vbuffer->GetLength() < vertices.size()){
				// バッファの再確保
				size_t newsize = vbuffer->GetLength();
				while (newsize < vertices.size()) newsize = 3 * newsize / 2;
				delete vbuffer; vbuffer = nullptr;
				vbuffer = new D3DVertexBuffer<Vertex>(core, vertices.data(), newsize, true);
			}
			else {
				// バッファを流用
				vbuffer->Update(vertices.data(), 0, vertices.size());
			}
		}
		vbuffer->Apply();

		if (!ibuffer){
			// バッファの作成
			ibuffer = new D3DIndexBuffer<>(core, indices.data(), indices.size(), true);
		}
		else {
			if (ibuffer->GetLength() < indices.size()){
				// バッファの再確保
				size_t newsize = ibuffer->GetLength();
				while (newsize < indices.size()) newsize = 3 * newsize / 2;
				delete ibuffer; ibuffer = nullptr;
				ibuffer = new D3DIndexBuffer<>(core, indices.data(), indices.size(), true);
			}
			else {
				// バッファを流用
				ibuffer->Update(indices.data(), 0, indices.size());
			}
		}
		ibuffer->Apply();

		ID3D11DepthStencilState* oldDss;
		UINT oldsref;
		core->GetDeviceContext()->OMGetDepthStencilState(&oldDss, &oldsref);
		Sprite::stencliState->Apply();

		// 描画
		core->SetPrimitiveTopology(D3DPrimitiveTopology::TriangleList);
		core->DrawIndexed((int)indices.size(), 0, 0);

		core->GetDeviceContext()->OMSetDepthStencilState(oldDss, oldsref);

		vertices.clear();
		indices.clear();
	}

	void Sprite::SetTexture(D3DTexture2D* tex) {
		if (tex) {
			this->texture = tex;
		}
		else {
			LOG_DBG("argument tex is nullptr.");
		}
	}

	D3DTexture2D* Sprite::GetTexture() {
		return this->texture;
	}

	void Sprite::SetSampler(D3DSampler* smp) {
		if (smp) {
			this->sampler = smp;
		}
		else {
			this->sampler = defaultSampler;
		}
	}

	D3DSampler* Sprite::GetSampler() {
		return this->sampler == defaultSampler ? nullptr : this->sampler;
	}

	void Sprite::SetEffector(Effector* e) {
		if (e) {
			this->effector = e;
		}
		else {
			this->effector = defaultEffector;
		}
	}

	Effector* Sprite::GetEffector() {
		return this->effector == defaultEffector ? nullptr : this->effector;
	}

	void Sprite::SetTranslator(Translator* t) {
		if (t) {
			this->translator = t;
		}
		else {
			this->translator = defaultTranslator;
		}
	}

	Translator* Sprite::GetTranslator() {
		return this->translator == defaultTranslator ? nullptr : this->translator;
	}




#pragma region Static Fields
	D3DCore* Sprite::core;
	D3DInputLayout *Sprite::inputLayout;

	DefaultTranslator *Sprite::defaultTranslator;
	DefaultEffector *Sprite::defaultEffector;

	D3DSampler* Sprite::defaultSampler;
	D3DConstantBuffer<ScreenSize> *Sprite::cbScreenSize;
	D3DConstantBuffer<TextureSize> *Sprite::cbTextureSize;

	D3DStencilState* Sprite::stencliState;

#pragma endregion

}



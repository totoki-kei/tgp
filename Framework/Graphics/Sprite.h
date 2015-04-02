#pragma once
#include "D3DCore.h"
#include "D3DInputLayout.h"
#include "D3DTexture.h"
#include "D3DSampler.h"
#include "D3DStencilState.h"
#include "D3DVertexBuffer.h"
#include "D3DIndexBuffer.h"
#include "D3DConstantBuffer.h"

namespace Sprite {
	__declspec(align(16))
	struct Vertex {
		XMFLOAT4 position;
		XMFLOAT2 texcoord;
		static D3D11_INPUT_ELEMENT_DESC* GetInputElementDesc();
	};

	__declspec(align(16))
	struct ScreenSize {
		uint32_t Width;
		uint32_t Height;
	};

	__declspec(align(16))
	struct TextureSize {
		uint32_t Width;
		uint32_t Height;
	};
}

#include "Translator.h"
#include "Effector.h"

namespace Sprite {



	class DefaultEffector;
	class DefaultTranslator;

	class Sprite {
	public:
		static void InitializeSharedResource(D3DCore* core);

	private:
		static D3DCore* core;
		static D3DInputLayout *inputLayout;

		//static Shaders::VertexShader *vsTransform;
		static DefaultTranslator *defaultTranslator;
		static DefaultEffector *defaultEffector;

		static D3DConstantBuffer<ScreenSize> *cbScreenSize;
		static D3DConstantBuffer<TextureSize> *cbTextureSize;

		static D3DStencilState* stencliState;


		static D3DSampler* defaultSampler;

		D3DTexture2D* texture;
		D3DSampler* sampler;
		Translator* translator;
		Effector* effector;

		std::vector<Vertex> vertices;
		std::vector<unsigned short> indices;

		D3DVertexBuffer<Vertex> *vbuffer;
		D3DIndexBuffer<> *ibuffer;

	public:

		Sprite(D3DTexture2D*, D3DSampler* = nullptr, Translator* = nullptr, Effector* = nullptr);
		~Sprite();

		void SetTexture(D3DTexture2D*);
		D3DTexture2D * GetTexture();
		void SetSampler(D3DSampler*);
		D3DSampler* GetSampler();
		void SetEffector(Effector*);
		Effector* GetEffector();
		void SetTranslator(Translator*);
		Translator* GetTranslator();

		void ReserveBuffer(int quadCount);

		void DrawQuad();
		void DrawQuad(const float x1, const float y1, const float x2, const float y2);
		void DrawQuad(const XMFLOAT4& left_top, const XMFLOAT4& left_bottom, const XMFLOAT4& right_top, const XMFLOAT4& right_bottom);
		void DrawQuad(const XMFLOAT4*);
		void DrawQuad(const Vertex& left_top, const Vertex& left_bottom, const Vertex& right_top, const Vertex& right_bottom);
		void DrawQuad(const Vertex*);

		void Flush();
	};

}

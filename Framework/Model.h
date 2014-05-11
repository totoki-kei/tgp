#pragma once

#include "D3DCore.h"
#include "D3DBuffer.h"
#include "D3DInputLayout.h"
#include "D3DRasterizer.h"

#include <vector>
#include <memory>

namespace Models{
	struct Vertex;
	struct SceneParameter;
	struct ObjectParameter;
	struct SubsetParameter;
	class ModelSubset;
	class Model;
}

namespace Models {
using std::vector;
using std::shared_ptr;

	struct Vertex {
		XMFLOAT4 position;
		XMFLOAT4 color;
		XMFLOAT4 emit;

		static D3D11_INPUT_ELEMENT_DESC* GetInputElementDesc();
		static int GetInputElementDescCount();

		Vertex(const XMFLOAT4& pos, const XMFLOAT4& color, const XMFLOAT4& emit);
		Vertex();

		typedef D3DVertexBuffer<Vertex> buffer_t;
	};


	struct SceneParameter {
		XMMATRIX View;
		XMMATRIX Projection;
		XMFLOAT4 LightDirection;
		XMFLOAT4 LightColor;
	};

	__declspec(align(16))
	struct ObjectParameter {
		XMMATRIX World;
	};

	__declspec(align(16))
	struct SubsetParameter{
		XMFLOAT4 BaseColor;
		XMFLOAT4 AlphaBalance;
	};

	enum ColoringType {
		COLORING_NORMAL,
		COLORING_EMIT,
		COLORING_LIGHTED,
	};

	class ModelSubset {
	public:
		typedef D3DIndexBuffer<> IndexBuffer;
		typedef D3DConstantBuffer<SubsetParameter> ConstantBuffer;
	private:
		D3DIndexBuffer<> indexBuffer;
		D3DConstantBuffer<SubsetParameter> subsetParam;
		bool isWireframe;
		ColoringType coloringType;

	public:
		ModelSubset(const int indexCount, const D3DIndexBuffer<>::index_t *indices, const SubsetParameter* param = nullptr, bool isWireframe = false, ColoringType type = COLORING_NORMAL);

		template<int Length>
		ModelSubset(D3DIndexBuffer<>::index_t(&indices)[Length], const SubsetParameter* param = nullptr, bool isWireframe = false, ColoringType type = COLORING_NORMAL)
		: ModelSubset(Length, indices, param, isWireframe, type) {	}

		~ModelSubset();

		inline bool GetWireframeMode(){ return isWireframe; }
		inline void SetWireframeMode(bool w){ isWireframe = w; }
		inline ColoringType GetColoringType(){ return coloringType; }
		inline void SetColoringType(ColoringType c){ coloringType = c; }
		
		inline void UpdateParameter(const SubsetParameter* p){ subsetParam.Update(p); }

		__declspec(property(get = GetWireframeMode, put = SetWireframeMode))
			bool $WireframeMode;

		__declspec(property(get = GetColoringType, put = SetColoringType))
			ColoringType $ColoringType;
		
		void Draw(Model* m);
	};




	class Model : public Resource
	{
		friend class ModelSubset;

	private:
		Vertex::buffer_t* vertexBuffer;
		int vertexCount;

		vector< shared_ptr<ModelSubset> > subsets;
		

		static void NameToModelResource(const char* modelType, Model* m);

		static D3DCore* core;
		static D3DInputLayout *inputLayout;

		static D3DRasterizer *rasterizer;

		static Shaders::VertexShader *vsTransform;
		static Shaders::GeometryShader *gsEdge;
		static Shaders::PixelShader *psColoring;

		static Shaders::ClassLinkage *classLinkage;
		static Shaders::ClassInstance *instNormalColor;
		static Shaders::ClassInstance *instEmitColor;
		static Shaders::ClassInstance *instLightedColor;

		static D3DConstantBuffer<SceneParameter> *cbScene;
		static D3DConstantBuffer<ObjectParameter> *cbObject;
//		static D3DConstantBuffer<SubsetParameter> *cbSubset;


	public:
		static Model* Load(const TCHAR* file);
		static void InitializeSharedResource(D3DCore* core);
		static void ApplyEffect(ColoringType colortype);
		static inline void UpdateSceneParams(SceneParameter* sce){
			cbScene->Update(sce);
		}

		Model(Vertex* vertices, int vertexCount);
		~Model();

		void Draw();
		void DrawSubset(int subsetIndex);

		inline void AddSubset(shared_ptr<ModelSubset> s){
			subsets.push_back(s);
		}

		inline void UpdateObjectParams(ObjectParameter* obj){
			cbObject->Update(obj);
		}
	};

}

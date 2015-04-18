#pragma once

#include "D3DCore.h"
#include "D3DVertexBuffer.h"
#include "D3DIndexBuffer.h"
#include "D3DConstantBuffer.h"
#include "D3DShaderResourceBuffer.h"
#include "D3DInputLayout.h"
#include "D3DRasterizer.h"

#include <vector>
#include <list>
#include <memory>

namespace Models{
	enum MaxConstans {
		InstanceCount = 8192,
		MaterialCount = 32,
		PointLightCount = 16,
	};

	struct Vertex;
	union PointLightData;
	struct InstanceData;
	struct MaterialData;
	struct SceneParameter;
	struct InstanceSetParameter;
	class ModelSubset;
	class Model;
}

namespace Models {
	using std::vector;
	using std::shared_ptr;

	// ポイントライト構造体
	SHADERDATA union PointLightData {
		struct {
			// 点光源のワールド座標
			float3 Position;
			// 点光源の最大到達距離
			float Distance;
			// 点光源の色(Alphaは強さ)
			float4 LightColor;
		};
		float rawvalue[3 + 1 + 4];
	};

	// インスタンス情報構造体
	SHADERDATA struct InstanceData {
		// ワールド行列
		SHADERDATA matrix World;
		// マテリアルセットの番号
		SHADERDATA struct ParamSet {
			float Index;
			float Blend;
			float LineWidth;
			float Alpha;

			ParamSet() = default;
		} Params[4];

#pragma warning(push)
#pragma warning(disable:4351)
		inline InstanceData() : Params{} {
			World = XMMatrixIdentity();
		}
#pragma warning(pop)
	};


	// マテリアル情報構造体
	SHADERDATA struct MaterialData {
		// 色
		float4 Color;
		//// 色割合(0以下の場合は未使用とみなす)
		//float Blend;
		//// 線の幅
		//float LineWidth;

		MaterialData(float4 color) : Color{ color } {}
	};

	// 頂点構造
	struct Vertex {
		float4 position;
		float4 color;
		float3 normal;

		struct ExtraInfo {
			uint32_t paramIndex;

			ExtraInfo() : paramIndex{ 0 } {};

			template <typename IntT>
			ExtraInfo(const IntT idx) : paramIndex{ idx } {}
		};

		static const D3D11_INPUT_ELEMENT_DESC* GetInputElementDesc();
		static int GetInputElementDescCount();

		Vertex(const XMFLOAT4& pos_, const XMFLOAT4& color_, const XMFLOAT3& normal_);
		Vertex();

		typedef D3DVertexBuffer<Vertex> buffer_t;
		typedef D3DVertexBuffer<ExtraInfo> buffer_extra_t;
	};


	// シーン固有の定数
	struct SceneParameter{
		SHADERDATA matrix View;
		SHADERDATA matrix Projection;
		SHADERDATA float4 AmbientColor;
		SHADERDATA float3 LightDirection;
		SHADERDATA float4 LightColor;

		SHADERDATA PointLightData PointLights[PointLightCount];
	};

	//// モデル単位の定数
	//struct InstanceSetParameter{
	//	SHADERDATA InstanceData Instance[InstanceCount];
	//};

	//// InstanceIDとInstance番号の対応表
	//SHADERDATA struct InstanceMapParameter {
	//	uint4 IDMap[InstanceCount];
	//};

	// マテリアル
	struct MaterialSetParameter {
		SHADERDATA MaterialData Materials[MaterialCount];
	};


	class Model : public Resource
	{

	private:
		Vertex::buffer_t* vertexBuffer;
		Vertex::buffer_extra_t* vertexExtraBuffer;

		D3DIndexBuffer<> *solidIndexBuffer;
		D3DIndexBuffer<> *edgeIndexBuffer;

		//D3DBuffer::Mapper<InstanceSetParameter> mInstanceSet;
		//D3DConstantBuffer<InstanceSetParameter> *cbInstanceSet;
		//std::vector<D3DConstantBuffer<InstanceSetParameter> *> cbInstanceSetFree;
		//std::vector<D3DConstantBuffer<InstanceSetParameter> *> cbInstanceSetOccupied;
		D3DBuffer::Mapper<InstanceData> mInstanceBuffer;
		D3DShaderResourceBuffer<InstanceData, InstanceCount> *sbInstanceBuffer;

		std::unique_ptr<InstanceData> defaultInstanceParams;

		D3DBuffer::Mapper<MaterialSetParameter> mMaterialSet;
		D3DConstantBuffer<MaterialSetParameter> *cbMaterialSet;

		int materialCount;
		int instanceCount;
		int pointLightCount;

		bool solidEnabled, edgeEnabled;
		bool edgeWidthEnabled;
		bool flatShading;
		//D3DShaderResourceBuffer<float, InstanceCount>* idMap;
		
		static void NameToModelResource(Model* m);

		static D3DCore* core;
		static D3DInputLayout *inputLayout;

		static D3DRasterizer *rasterizer;

		static Shaders::VertexShader *vsTransform;
		static Shaders::VertexShader *vsTransformSingle;
		static Shaders::GeometryShader *gsLine;
		static Shaders::GeometryShader *gsFlat;
		static Shaders::PixelShader *psColoring;
		static Shaders::PixelShader *psNoLight;

		static D3DBuffer::Mapper<SceneParameter> mScene;
		static D3DConstantBuffer<SceneParameter> *cbScene;

	public:
		static Model* Load(const TCHAR* file, bool solid = true, bool edge = true);
		static void InitializeSharedResource(D3DCore* core);
		static void ApplyEffect();
		static void ApplyEffect_NoInstancing();

		static inline SceneParameter& GetSceneParam() {
			if (!mScene) mScene.Map(cbScene);
			return *mScene;
		}


		//inline void UpdateObjectParams(){
		//	cbInstanceSet->Update(&instanceSet);
		//}
		//inline void UpdateMaterialParams(){
		//	cbMaterial->Update(&material);
		//}

		Model(Vertex* vertices, size_t vertexCount, Vertex::ExtraInfo* extra,
			  D3DIndexBuffer<>::index_t* solidIndices, size_t solidIndicesCount,
			  D3DIndexBuffer<>::index_t* edgeIndices, size_t edgeIndicesCount,
			  InstanceData* defaultInstanceData);
		~Model();


		void Draw(const InstanceData& instData);
		InstanceData& ReserveDraw();
		void Flush();
		void Flush(bool solid, bool edge);

		void SetMaterial(int index, MaterialData& mat);
		
		void SetFlushMode(bool solid, bool edge) {
			this->solidEnabled = solid;
			this->edgeEnabled = edge;
		}
		void GetFlushMode(bool* outSolid, bool* outEdge) {
			if (outSolid)*outSolid = solidEnabled;
			if (outEdge) *outEdge = edgeEnabled;
		}

		void SetEdgeWidthEnableFlag(bool enabled) {
			this->edgeWidthEnabled = enabled;
		}
		bool GetEdgeWidthEnableFlag() {
			return this->edgeWidthEnabled;
		}

		void SetFlatShadingFlag(bool enabled) {
			this->flatShading = enabled;
		}
		bool GetFlatShadingFlag() {
			return this->flatShading;
		}
	};

}

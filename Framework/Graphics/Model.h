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

	// �|�C���g���C�g�\����
	SHADERDATA union PointLightData {
		struct {
			// �_�����̃��[���h���W
			float3 Position;
			// �_�����̍ő哞�B����
			float Distance;
			// �_�����̐F(Alpha�͋���)
			float4 LightColor;
		};
		float rawvalue[3 + 1 + 4];
	};

	// �C���X�^���X���\����
	SHADERDATA struct InstanceData {
		// ���[���h�s��
		SHADERDATA matrix World;
		// �}�e���A���Z�b�g�̔ԍ�
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


	// �}�e���A�����\����
	SHADERDATA struct MaterialData {
		// �F
		float4 Color;
		//// �F����(0�ȉ��̏ꍇ�͖��g�p�Ƃ݂Ȃ�)
		//float Blend;
		//// ���̕�
		//float LineWidth;

		MaterialData(float4 color) : Color{ color } {}
	};

	// ���_�\��
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


	// �V�[���ŗL�̒萔
	struct SceneParameter{
		SHADERDATA matrix View;
		SHADERDATA matrix Projection;
		SHADERDATA float4 AmbientColor;
		SHADERDATA float3 LightDirection;
		SHADERDATA float4 LightColor;

		SHADERDATA PointLightData PointLights[PointLightCount];
	};

	//// ���f���P�ʂ̒萔
	//struct InstanceSetParameter{
	//	SHADERDATA InstanceData Instance[InstanceCount];
	//};

	//// InstanceID��Instance�ԍ��̑Ή��\
	//SHADERDATA struct InstanceMapParameter {
	//	uint4 IDMap[InstanceCount];
	//};

	// �}�e���A��
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

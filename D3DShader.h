#pragma once

namespace Shaders {
	class VertexShader;
	class PixelShader;
	class GeometryShader;
	class ComputeShader;


	enum class ShaderFlag{
		Vertex = 0x01,
		Geometry = 0x02,
		Pixel = 0x04,
		Compute = 0x08,

		All = Vertex | Geometry | Pixel | Compute,
	};
}

#include "Resource.h"
#include "D3DCore.h"
#include "D3DBuffer.h"

#include "Utility.h"


namespace Shaders {

	//class ShaderInterface {
	//public:
	//	virtual void Apply() = 0;
	//	virtual void SetConstantBuffer(int index, D3DBuffer* buffer) = 0;
	//};

	class VertexShader : public Resource {

	public:
		typedef ID3D11VertexShader shader_t;

		VertexShader(D3DCore* core, const BYTE* data, SIZE_T dataSize);
		~VertexShader();
		void Apply();

	private:
		shader_t* shader;
		D3DCore* core;
		const BYTE* bytecode;
		SIZE_T bytecodeSize;
	public:
		inline shader_t* GetShader() const { return shader; }
		inline D3DCore* GetCore() const { return core; }
		inline const BYTE* GetBytecode(int* outSize) const { outSize ? (*outSize = bytecodeSize) : 0; return bytecode; }

		// Resource class member override
		bool isDisposed();
		void Dispose();
	};

	class PixelShader : public Resource {
	public:
		typedef ID3D11PixelShader shader_t;

		PixelShader(D3DCore* core, const BYTE* data, SIZE_T dataSize);
		~PixelShader();
		void Apply();

	private:
		shader_t* shader;
		D3DCore* core;
		const BYTE* bytecode;
		SIZE_T bytecodeSize;
	public:
		inline shader_t* GetShader() const { return shader; }
		inline D3DCore* GetCore() const { return core; }
		inline const BYTE* GetBytecode(int* outSize) const { outSize ? (*outSize = bytecodeSize) : 0; return bytecode; }

		// Resource class member override
		bool isDisposed();
		void Dispose();
	};

	class GeometryShader : public Resource {
	public:
		typedef ID3D11GeometryShader shader_t;

		GeometryShader(D3DCore* core, const BYTE* data, SIZE_T dataSize);
		~GeometryShader();
		void Apply();

	private:
		shader_t* shader;
		D3DCore* core;
		const BYTE* bytecode;
		SIZE_T bytecodeSize;
	public:
		inline shader_t* GetShader() const { return shader; }
		inline D3DCore* GetCore() const { return core; }
		inline const BYTE* GetBytecode(int* outSize) const { outSize ? (*outSize = bytecodeSize) : 0; return bytecode; }

		// Resource class member override
		bool isDisposed();
		void Dispose();
	};

	class ComputeShader : public Resource {
	public:
		typedef ID3D11ComputeShader shader_t;

		ComputeShader(D3DCore* core, const BYTE* data, SIZE_T dataSize);
		~ComputeShader();
		void Apply();

	private:
		shader_t* shader;
		D3DCore* core;
		const BYTE* bytecode;
		SIZE_T bytecodeSize;
	public:
		inline shader_t* GetShader() const { return shader; }
		inline D3DCore* GetCore() const { return core; }
		inline const BYTE* GetBytecode(int* outSize) const { outSize ? (*outSize = bytecodeSize) : 0; return bytecode; }

		// Resource class member override
		bool isDisposed();
		void Dispose();
	};

	template <typename ShaderT>
	ShaderT* Load(D3DCore* core, const TCHAR* filename) {
		int size = -1;
		BYTE* data = LoadFileToMemory(filename, &size);
		if (!data) {
			// “Ç‚Ýž‚ß‚È‚©‚Á‚½
		}

		ShaderT* ret = new ShaderT(core, data, size);
		ret->AddResource(PtrToRes(data));
		return ret;
	}

	inline bool CheckFlag(ShaderFlag left, ShaderFlag right){
		return (((int)left) & ((int)right)) != 0;
	}
};

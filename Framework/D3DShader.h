#pragma once

namespace Shaders {
	class VertexShader;
	class PixelShader;
	class GeometryShader;
	class ComputeShader;

	class ClassLinkage;
	class ClassInstance;
	class ClassInstanceList;

	enum class ShaderFlag : unsigned int {
		Vertex = 0x01,
		Geometry = 0x02,
		Pixel = 0x04,
		Compute = 0x08,

		Render = Vertex | Geometry | Pixel,
		All = Vertex | Geometry | Pixel | Compute,
	};
}

#include "Resource.h"
#include "D3DCore.h"
#include "D3DBuffer.h"

#include "Utility.h"


namespace Shaders {

	class VertexShader : public Resource {

	public:
		typedef ID3D11VertexShader shader_t;

		VertexShader(D3DCore* core, const BYTE* data, SIZE_T dataSize, ClassLinkage* ClassLinkage = nullptr);
		~VertexShader();
		void Apply();
		void Apply(const ClassInstanceList&);

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

		PixelShader(D3DCore* core, const BYTE* data, SIZE_T dataSize, ClassLinkage* linkage = nullptr);
		~PixelShader();
		void Apply();
		void Apply(const ClassInstanceList&);

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

		GeometryShader(D3DCore* core, const BYTE* data, SIZE_T dataSize, ClassLinkage* linkage = nullptr);
		~GeometryShader();
		void Apply();
		void Apply(const ClassInstanceList&);

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

		ComputeShader(D3DCore* core, const BYTE* data, SIZE_T dataSize, ClassLinkage* linkage = nullptr);
		~ComputeShader();
		void Apply();
		void Apply(const ClassInstanceList&);

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
	ShaderT* Load(D3DCore* core, const TCHAR* filename, ClassLinkage* linkage = nullptr) {
		int size = -1;
		BYTE* data = LoadFileToMemory(filename, &size);
		if (!data) {
			// “Ç‚Ýž‚ß‚È‚©‚Á‚½
			return nullptr;
		}

		ShaderT* ret = new ShaderT(core, data, size);
		ret->AddResource(PtrToRes(data));

#ifdef _DEBUG
#ifdef UNICODE
		{
			size_t clen = wcslen(filename) * 2;
			char* name = new char[clen];
			size_t len;
			wcstombs_s(&len, name, clen, filename, clen);
			NameToResource(ret->GetShader(), name);
			delete name;
		}
#else
		NameToResource(ret, filename);
#endif
#endif
		return ret;
	}

	inline bool CheckFlag(ShaderFlag left, ShaderFlag right){
		return (((int)left) & ((int)right)) != 0;
	}

	void Unapply(D3DCore* core, ShaderFlag shader);


	class ClassLinkage : public Resource {
		ID3D11ClassLinkage *linkage;

	public:
		inline ID3D11ClassLinkage* GetLinkage() { return linkage; }
		__declspec(property(get = GetLinkage))
			ID3D11ClassLinkage* $Linkage;

		ClassLinkage(D3DCore *core);

		ClassInstance* Create(const char* className);

		ClassInstance* Get(const char* instanceName, int index = 0);

	};

	class ClassInstance : public Resource {
		ID3D11ClassInstance* inst;

	public:
		inline ID3D11ClassInstance* GetInstance(){ return inst; }
		__declspec(property(get = GetInstance))
			ID3D11ClassInstance* $Instance;

		ClassInstance(ID3D11ClassInstance* inst);
	};

	class ClassInstanceList {
	public:
		int Count;
		ID3D11ClassInstance** PtrArray;

		inline ClassInstanceList(std::initializer_list<ClassInstance*> clsinstList) : Count(0), PtrArray(nullptr) {
			Count = clsinstList.size();
			PtrArray = new ID3D11ClassInstance*[Count];
			
			auto p = PtrArray;
			for (auto c : clsinstList){
				*p++ = c->$Instance;
			}
		}
		inline ~ClassInstanceList(){
			if (PtrArray) delete[] PtrArray;
		}
	};

};

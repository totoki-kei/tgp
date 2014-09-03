#if !defined(HLSL_PL_EFFECTOR)
#define HLSL_PL_EFFECTOR


struct VS_IN {
	uint id : SV_VertexID;
	float4 pos : POSITION; // xyzw;
	float4 uvwh : UVWH; // texture left, top, width, height


};

struct VS_OUT {
	uint id : ID;
};

typedef VS_OUT GS_IN;

struct GS_OUT {
	float4 position : SV_Position;
	float2 uv : TEXCOORD;
};

typedef GS_OUT PS_IN;

struct PS_OUT {
	float4 color : SV_Target;
};

#endif // HLSL_PL_EFFECTOR

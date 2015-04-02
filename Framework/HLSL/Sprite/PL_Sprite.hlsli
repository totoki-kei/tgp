#if !defined(HLSL_PL_SPRITE)
#define HLSL_PL_SPRITE

struct VS_IN {
	float4 pos : SV_Position; // xyzw;
	float2 uv : TEXCOORD; // texture left, top, width, height
};

typedef VS_IN VS_OUT;

typedef VS_OUT PS_IN;

struct PS_OUT {
	float4 color : SV_Target;
};

#endif // HLSL_PL_SPRITE

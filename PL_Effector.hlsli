Texture2D Texture;
Texture2D PatternMap;

SamplerState Sampler;

struct VS_IN {
	uint id : SV_VertexID;
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

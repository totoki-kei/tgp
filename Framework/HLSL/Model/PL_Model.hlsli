#if !defined(HLSL_PL_MODEL)
#define HLSL_PL_MODEL

// モデル描画パイプライン ヘッダ

cbuffer Scene : register(b0) {
	float4x4 View;
	float4x4 Projection;

	float3 LightDirection;
	float4 LightColor;
}

cbuffer Object : register(b1) {
	float4x4 World;
};

cbuffer Subset : register(b2) {
	float4 BaseColor;
	float2 EdgeGradient;
};


struct VS_IN {
	// 座標
	float4 position : POSITION;
	// 描画色
	float4 color : COLOR;
	// 発光色
	float4 emit : EMIT;
};

typedef VS_IN VS_OUT;

typedef VS_OUT GS_IN;

struct GS_OUT {
	// 変換済み座標
	float4 position : SV_Position;
	// 描画色
	float4 color : COLOR;
	// 発光色
	float4 emit : EMIT;
	// エッジ係数
	float4 edge : EDGE;
	// ライトの係数
	float light : LIGHT;
};

typedef GS_OUT PS_IN;


struct PS_OUT {
	// 結果
	float4 color : SV_Target;
};

#endif // HLSL_PL_MODEL

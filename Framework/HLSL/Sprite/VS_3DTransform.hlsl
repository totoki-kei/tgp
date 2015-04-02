#include "PL_Sprite.hlsli"

cbuffer Transform : register(b0) {
	float4x4 WVP;
}

cbuffer TextureSize : register(b1) {
	uint TextureWidth;
	uint TextureHeight;
};

VS_OUT main(VS_IN input) {
	VS_OUT output;

	output.pos = mul(WVP, input.pos);
	output.uv = input.uv / float2(TextureWidth, TextureHeight);

	return output;
}

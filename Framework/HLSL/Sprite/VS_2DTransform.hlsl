#include "PL_Sprite.hlsli"

cbuffer ScreenSize : register(b0){
	uint ScreenWidth;
	uint ScreenHeight;
};

cbuffer TextureSize : register(b1) {
	uint TextureWidth;
	uint TextureHeight;
};

VS_OUT main( VS_IN input )
{
	VS_OUT output;

	// pos
	// X : (0, ScreenWidth) -> (-1, 1)
	// Y : (0, ScreenHeight) -> (1, -1)
	output.pos.xy = (2 * input.pos.xy) / float2(ScreenWidth, ScreenHeight) - float2(1, 1);
	output.pos.y *= -1;
	output.pos.zw = input.pos.zw;

	// uv
	// X : (0, TextureWidth) -> (0, 1)
	// Y : (0, TextureHeight) -> (0, 1)
	output.uv = input.uv / float2(TextureWidth, TextureHeight);

	return output;
}

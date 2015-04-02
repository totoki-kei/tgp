#include "PL_Sprite.hlsli"

Texture2D Texture;
SamplerState Sampler;

cbuffer Color {
	float4 BodyColor;
	float4 EdgeColor;
};


PS_OUT main(PS_IN input) {
	PS_OUT output;
	float4 c;

	c = Texture.Sample(Sampler, input.uv);
	output.color = (BodyColor * c.r * c.a) + (EdgeColor * c.g * c.a);
	
	return output;
}


#include "PL_Sprite.hlsli"

Texture2D Texture;
SamplerState Sampler;

PS_OUT main(PS_IN input) {
	PS_OUT output;

	output.color = Texture.Sample(Sampler, input.uv);

	//
	//float2 dx = ddx(input.uv);
	//float2 dy = ddy(input.uv);
	//output.color = max(output.color, Texture.Sample(Sampler, input.uv + dx));
	//output.color = max(output.color, Texture.Sample(Sampler, input.uv - dx));
	//output.color = max(output.color, Texture.Sample(Sampler, input.uv + dy));
	//output.color = max(output.color, Texture.Sample(Sampler, input.uv - dy));
	//

	return output;
}


#if !defined(HLSL_GAUSSIAN_EFFECT)
#define HLSL_GAUSSIAN_EFFECT

Texture2D Texture;
SamplerState Sampler;

cbuffer TextureData {
	uint2 size;
};


PS_OUT gauss(PS_IN input, float2 delta) {
	PS_OUT output;

	float kernel[5] = {
		70,
		56,
		28,
		8,
		1,
	};

	float kernelDivider = 256;

	output.color = float4(0, 0, 0, 0);

	output.color += Texture.Sample(Sampler, input.uv - 4 * delta) * kernel[4];
	output.color += Texture.Sample(Sampler, input.uv - 3 * delta) * kernel[3];
	output.color += Texture.Sample(Sampler, input.uv - 2 * delta) * kernel[2];
	output.color += Texture.Sample(Sampler, input.uv - 1 * delta) * kernel[1];
	output.color += Texture.Sample(Sampler, input.uv)             * kernel[0];
	output.color += Texture.Sample(Sampler, input.uv + 1 * delta) * kernel[1];
	output.color += Texture.Sample(Sampler, input.uv + 2 * delta) * kernel[2];
	output.color += Texture.Sample(Sampler, input.uv + 3 * delta) * kernel[3];
	output.color += Texture.Sample(Sampler, input.uv + 4 * delta) * kernel[4];
	output.color /= kernelDivider;
	return output;
}



#endif // HLSL_GAUSSIAN_EFFECT
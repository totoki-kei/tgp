#include "PL_Sprite.hlsli"

Texture2D Texture;
SamplerState Sampler;

cbuffer FilterParameter {
	float Threshold;
};

PS_OUT main(PS_IN input) {
	PS_OUT output;
	float4 c;
	float3 rgb;

	float luminance;

	c = Texture.Sample(Sampler, input.uv);
	rgb = c.rgb;

	luminance = dot(rgb, float3(0.299, 0.587, 0.114));
	rgb *= max(luminance - Threshold, 0.0f) / luminance;

	//rgb = saturate((rgb - inMin) / (inMax - inMin));
	//rgb = outMin + rgb * (outMax - outMin);

	output.color = float4(rgb, 1);
	return output;
}

/*
// Calculate perceptive luminance
float luminance = dot(color, float3(0.299, 0.587, 0.114));

// Apply threshold
color *= max(luminance - BrightPassThreshold, 0.0f) / luminance;

return float4(color, 1.0);
*/
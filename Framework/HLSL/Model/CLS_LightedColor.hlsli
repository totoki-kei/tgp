#if !defined(HLSL_CLS_LIGHTCOLOR)
#define HLSL_CLS_LIGHTCOLOR

#include "PL_Model.hlsli"
#include "IF_PixelColor.hlsli"
#include "../Functions.hlsli"

class LightedColor : IPixelColor {
	float4 GetColor(PS_IN input) {
		float4 output;
		float alpha;

		alpha = Edge(input.edge);

		output      = input.color;
		output.rgb *= LightColor.rgb * input.light;
		output.a *= AlphaBalance.z;

		output = MidairBlend(output, float4(input.emit.rgb * alpha, alpha) * AlphaBalance.w);

		output *= BaseColor;

		return output;
	}
};

#endif // HLSL_CLS_LIGHTCOLOR

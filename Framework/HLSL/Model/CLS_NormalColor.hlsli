#if !defined(HLSL_CLS_NORMALCOLOR)
#define HLSL_CLS_NORMALCOLOR

#include "PL_Model.hlsli"
#include "IF_PixelColor.hlsli"
#include "../Functions.hlsli"

class NormalColor : IPixelColor  {
	float4 GetColor(PS_IN input) {
		float4 output;
		float4 lower, upper;

		lower.rgb = input.color.rgb;
		lower.a = input.color.a * AlphaBalance.z;

		upper.rgb = input.emit.rgb;
		upper.a = Edge(input.edge);
		upper.a *= AlphaBalance.w;

		output = MidairBlend(lower, upper);

		output *= BaseColor;

		return output;
	}
};

#endif // HLSL_CLS_NORMALCOLOR

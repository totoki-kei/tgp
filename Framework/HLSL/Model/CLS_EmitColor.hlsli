#if !defined(HLSL_CLS_EMITCOLOR)
#define HLSL_CLS_EMITCOLOR

#include "PL_Model.hlsli"
#include "IF_PixelColor.hlsli"
#include "../Functions.hlsli"

class EmitColor : IPixelColor  {
	float4 GetColor(PS_IN input) {
		float4 output;
		float4 lower, upper;

		lower.rgb = input.emit.rgb;
		lower.a = input.emit.a * AlphaBalance.z;

		upper.rgb = input.color.rgb;
		upper.a = Edge(input.edge);
		upper.a *= AlphaBalance.w;

		output = MidairBlend(lower, upper);

		output *= BaseColor;

		return output;
	}
};

#endif // HLSL_CLS_EMITCOLOR

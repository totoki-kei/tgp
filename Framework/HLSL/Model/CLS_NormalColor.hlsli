#if !defined(HLSL_CLS_NORMALCOLOR)
#define HLSL_CLS_NORMALCOLOR

#include "PL_Model.hlsli"
#include "IF_PixelColor.hlsli"

class NormalColor : IPixelColor  {
	float4 GetColor(PS_IN input) {
		float4 output;
		float alpha;

		output = input.color;

		alpha
			= input.edge.x * input.edge.x
			+ input.edge.y * input.edge.y
			+ input.edge.z * input.edge.z
			+ input.edge.w * input.edge.w;

		output.a *= alpha;

		return output;
	}
};

#endif // HLSL_CLS_NORMALCOLOR

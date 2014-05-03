#if !defined(HLSL_IF_PIXELCOLOR)
#define HLSL_IF_PIXELCOLOR

#include "PL_Model.hlsli"

interface IPixelColor {
	float4 GetColor(PS_IN input);
};

#endif // HLSL_IF_PIXELCOLOR

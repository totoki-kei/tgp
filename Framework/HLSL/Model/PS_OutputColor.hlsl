#include "PL_Model.hlsli"
#include "CLS_NormalColor.hlsli"
#include "CLS_EmitColor.hlsli"
#include "CLS_LightedColor.hlsli"

IPixelColor colorGenerator;

PS_OUT main(PS_IN input)
{
	PS_OUT output;
	output.color = colorGenerator.GetColor(input);
	return output;
}

#include "PL_Effector.hlsli"

VS_OUT main( VS_IN input)
{
	VS_OUT output;
	output.id = input.id;
	return output;
}

#include "PL_Model.hlsli"

VS_OUT main( VS_IN input )
{
	VS_OUT output;

	float4 worldPosition, viewPosition, projPosition;
	//worldPosition = mul(input.position, World);
	//viewPosition = mul(worldPosition, View);
	//projPosition = mul(viewPosition, Projection);
	worldPosition = mul(World, input.position);
	viewPosition = mul(View, worldPosition);
	projPosition = mul(Projection, viewPosition);

	output.position = projPosition;
	output.color = input.color;
	output.emit = input.emit;

	return output;
}

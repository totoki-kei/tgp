#include "PL_Model.hlsli"

PS_OUT main(PS_IN input) {
	PS_OUT output;
	float alpha;

	output.color = input.color;

	alpha
		= input.edge.x * input.edge.x
		+ input.edge.y * input.edge.y
		+ input.edge.z * input.edge.z 
		+ input.edge.w * input.edge.w;

	output.color.a *= alpha;

	return output;
}

#include "PL_Effector.hlsli"

PS_OUT main(PS_IN input) {
	PS_OUT output;
	output.color = Texture.Sample(Sampler, input.uv);
	return output; 
}


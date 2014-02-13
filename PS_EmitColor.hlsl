#include "PL_Model.hlsli"

PS_OUT main(PS_IN input) {
	PS_OUT output;

	output.color = input.emit;
	return output;
}

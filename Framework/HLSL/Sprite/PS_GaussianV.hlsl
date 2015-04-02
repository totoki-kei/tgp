#include "PL_Sprite.hlsli"
#include "GaussianEffect.hlsli"

PS_OUT main(PS_IN input) {
	float2 d = float2(0, 1.0 / size.y);

	return gauss(input, d);
}

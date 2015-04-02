#include "PL_Sprite.hlsli"
#include "GaussianEffect.hlsli"

PS_OUT main(PS_IN input) {
	float2 d = float2(1.0 / size.x, 0);

	return gauss(input, d);
}

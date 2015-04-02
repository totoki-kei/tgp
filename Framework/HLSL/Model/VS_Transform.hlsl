#include "PL_Model.hlsli"
#include "Functions.hlsli"

// 頂点シェーダ

VS_OUT main(VS_IN input, uint id : SV_InstanceID)
{
	return Transform(input, id);
}

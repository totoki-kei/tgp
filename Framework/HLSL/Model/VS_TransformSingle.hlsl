#include "PL_Model.hlsli"
#include "Functions.hlsli"

// 頂点シェーダ
// ※ 単一インスタンス用に特殊化したもの

VS_OUT main(VS_IN input)
{
	return Transform(input, 0);
}

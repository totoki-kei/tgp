#include "PL_Model.hlsli"
#include "Functions.hlsli"

// ピクセルシェーダ
// ライティング計算あり

PS_OUT main(PS_IN input)
{
	PS_OUT output;

	// 初期色
	output.color = input.color;

	return output;
}


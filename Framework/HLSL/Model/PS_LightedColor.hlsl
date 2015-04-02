#include "PL_Model.hlsli"
#include "Functions.hlsli"

// ピクセルシェーダ
// ライティング計算あり

PS_OUT main(PS_IN input)
{
	PS_OUT output;

	float dirLightPower;
	float3 dirLightColor;
	float3 ptLightColor;

	// (1) 線形ライトの計算
	// 法線との内積から光の当たりの強さを計算
	dirLightPower = saturate(-dot(input.normal, LightDirection));
	// ライトの強さを乗算
	dirLightPower *= LightColor.a;
	// 算出した強さから、線形ライトの色成分を確定
	dirLightColor = LightColor.rgb * dirLightPower;

	// (2) ポイントライトの計算
	// 初期値の設定
	ptLightColor = float3(0, 0, 0);
	// 光源ごとにループ
	[unroll] for (int i = 0; i < POINTLIGHT_COUNT; i++){
		float3 distance;
		float dist2;
		float maxdist2 = PointLights[i].Distance * PointLights[i].Distance;
		
		float power;
		float4 ptcolor = PointLights[i].LightColor;

		if (ptcolor.a == 0) continue;
		
		// 点光源 -> ピクセル へのベクトルを計算
		distance = input.world - PointLights[i].Position;

		// 距離の2乗を計算
		dist2 = dot(distance, distance);

		// 距離が最大到達距離より大きい場合は以後の計算をスキップする
		if (dist2 > maxdist2){
			continue;
		}

		// ベクトルから反射の強さを計算
		power = saturate(-dot(input.normal, normalize(distance)));

		// 距離による減衰を反映(二乗比例)
		power *= ((maxdist2 - dist2) / maxdist2);

		// 色に設定されている強さを反映
		power *= ptcolor.a;

		// この光源によってもたらされる分を最終結果に加算
		// 白飛びなども可能なようにするため、特に上限は設けずに加算する
		ptLightColor += ptcolor.rgb * power;
	}

	// (3) 最終結果の計算
	output.color = float4(0, 0, 0, input.color.a);

	// 環境光
	output.color.rgb += input.color.rgb * (AmbientColor.rgb * AmbientColor.a);
	// ディレクショナルライト
	output.color.rgb += input.color.rgb * dirLightColor;
	// ポイントライト(加算)
	output.color.rgb += input.color.rgb * ptLightColor;


	return output;
}


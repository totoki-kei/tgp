#include "PL_Model.hlsli"
#include "Functions.hlsli"

// ジオメトリシェーダ
// 線の入力から、幅のある四角を出力する

[maxvertexcount(8)]
void main(
	line VS_OUT input[2],
	inout TriangleStream< VS_OUT > output
	)
{
	VS_OUT element;

	float3 normal;
	float3 widthDirection;
	float lineWidth0;
	float lineWidth1;

	// 法線を計算 : 2頂点の法線の平均を正規化したもの
	normal = input[1].normal + input[0].normal;
	if (length(normal) == 0) return;
	normal = normalize(normal);

	// 線の幅を取得
	lineWidth0 = input[0].width;
	lineWidth1 = input[1].width;
	if (lineWidth0 * lineWidth1 <= 0) return;

	// 法線と直線の方向から、線の太さ方向の正規化ベクトルを計算
	widthDirection = normalize(cross(normal, input[1].world - input[0].world));

	// 頂点の出力(両面の四角形)
	// (0) 共通情報
	element.normal = normal;
	element.width = 0;
	// (1) 左上
	element.world = input[0].world + widthDirection * lineWidth0;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[0].color * float4(2, 2, 2, 1);
	output.Append(element);
	// (2) 右上
	element.world = input[0].world - widthDirection * lineWidth0;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[0].color * float4(2, 2, 2, 1);
	output.Append(element);
	// (3) 左下
	element.world = input[1].world + widthDirection * lineWidth1;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[1].color * float4(2, 2, 2, 1);
	output.Append(element);
	// (4) 右下
	element.world = input[1].world - widthDirection * lineWidth1;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[1].color * float4(2, 2, 2, 1);
	output.Append(element);

	output.RestartStrip();

	// (0) 共通情報（法線を逆転）
	element.normal = -normal;
	// (3) 左下
	element.world = input[1].world + widthDirection * lineWidth1;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[1].color;
	output.Append(element);
	// (4) 右下
	element.world = input[1].world - widthDirection * lineWidth1;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[1].color;
	output.Append(element);
	// (1) 左上
	element.world = input[0].world + widthDirection * lineWidth0;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[0].color;
	output.Append(element);
	// (2) 右上
	element.world = input[0].world - widthDirection * lineWidth0;
	element.position = mul(Projection, mul(View, float4(element.world, 1)));
	element.color = input[0].color;
	output.Append(element);

	output.RestartStrip();

}

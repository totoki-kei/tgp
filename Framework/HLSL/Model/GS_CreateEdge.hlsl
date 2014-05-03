#include "PL_Model.hlsli"

[maxvertexcount(3)]
void main(
	triangle GS_IN input[3],
	inout TriangleStream< GS_OUT > output
	)
{
	GS_OUT element;

	float3 normal;
	float  dotval;

	// 法線を計算
	normal = normalize(cross(input[1].position.xyz - input[0].position.xyz, input[2].position.xyz - input[0].position.xyz));
	// 法線と光の方向との内積を計算
	dotval = dot(normal, normalize(LightDirection));
	// 0以下の場合は0に補正する
	element.light = clamp(dotval, 0, 1);

	element.position = input[0].position;
	element.color = input[0].color;
	element.emit = input[0].emit;
	element.edge = float4(1, 0, 0, 1) * EdgeGradient.xxxy;
	output.Append(element);

	element.position = input[1].position;
	element.color = input[1].color;
	element.emit = input[1].emit;
	element.edge = float4(0, 1, 0, 1) * EdgeGradient.xxxy;
	output.Append(element);

	element.position = input[2].position;
	element.color = input[2].color;
	element.emit = input[2].emit;
	element.edge = float4(0, 0, 1, 1) * EdgeGradient.xxxy;
	output.Append(element);
}

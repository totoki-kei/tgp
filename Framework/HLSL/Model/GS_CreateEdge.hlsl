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

	// �@�����v�Z
	normal = normalize(cross(input[1].position.xyz - input[0].position.xyz, input[2].position.xyz - input[0].position.xyz));
	// �@���ƌ��̕����Ƃ̓��ς��v�Z
	dotval = dot(normal, normalize(LightDirection));
	// 0�ȉ��̏ꍇ��0�ɕ␳����
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

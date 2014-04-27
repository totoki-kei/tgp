#include "PL_Model.hlsli"

[maxvertexcount(3)]
void main(
	triangle GS_IN input[3],
	inout TriangleStream< GS_OUT > output
	)
{
	GS_OUT element;

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

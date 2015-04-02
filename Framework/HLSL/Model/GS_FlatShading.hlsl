#include "PL_Model.hlsli"

[maxvertexcount(3)]
void main(
	triangle VS_OUT input[3],
	inout TriangleStream< VS_OUT > output
)
{
	// ¡‚Â‚¢‚Ä‚¢‚é–@üî•ñ‚ÍƒKƒ“–³‹‚µ‚Ä
	// À•W‚©‚çˆê‚©‚çŒvZ‚µ‚È‚¨‚·
	float3 normal;

	normal = cross(input[1].world - input[0].world, input[2].world - input[0].world);
	normal = normalize(normal);

	for (uint i = 0; i < 3; i++)
	{
		VS_OUT element;
		element = input[i];
		element.normal = normal;
		output.Append(element);
	}
}
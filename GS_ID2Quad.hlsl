#include "PL_Effector.hlsli"


[maxvertexcount(6)]
void main(point GS_IN id[1] : ID, inout TriangleStream<GS_OUT> outStream) {
	GS_OUT v[4];

	v[0].position = float4(0, 0, 0, 1);
	v[1].position = float4(0, 1, 0, 1);
	v[2].position = float4(1, 0, 0, 1);
	v[3].position = float4(0, 1, 0, 1);
	v[0].uv = v[0].position.xy;
	v[1].uv = v[1].position.xy;
	v[2].uv = v[2].position.xy;
	v[3].uv = v[3].position.xy;

	outStream.Append(v[0]);
	outStream.Append(v[1]);
	outStream.Append(v[2]);
	outStream.RestartStrip();

	outStream.Append(v[3]);
	outStream.Append(v[2]);
	outStream.Append(v[1]);
	outStream.RestartStrip();
}

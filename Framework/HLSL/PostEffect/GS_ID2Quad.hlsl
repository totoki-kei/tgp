#include "PL_Effector.hlsli"


[maxvertexcount(6)]
void main(point GS_IN id[1] : ID, inout TriangleStream<GS_OUT> outStream) {
	GS_OUT v[4];

	v[0].position = float4(-1, -1, 0, 1);
	v[1].position = float4(-1, 1, 0, 1);
	v[2].position = float4(1, -1, 0, 1);
	v[3].position = float4(1, 1, 0, 1);
	v[0].uv = float2(0, 1);
	v[1].uv = float2(0, 0);
	v[2].uv = float2(1, 1);
	v[3].uv = float2(1, 0);

	outStream.Append(v[0]);
	outStream.Append(v[1]);
	outStream.Append(v[2]);
	outStream.RestartStrip();

	outStream.Append(v[3]);
	outStream.Append(v[2]);
	outStream.Append(v[1]);
	outStream.RestartStrip();
}


//#include "Common.fx"

Texture2D Texture;
Texture2D PatternMap;

cbuffer GaussParams {
	int2 SampleArea;
	int2 SampleDensity;
};

struct PS_IN {
	float4 position : SV_Position;
	float2 uv : TEXCOORD;
};

uint VS_Default(uint id : SV_VertexID) : ID {
	return id;
}

[maxvertexcount(6)]
void GS_Default(point uint id[1] : ID, inout TriangleStream<PS_IN> outStream) {
	PS_IN v[4];

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

SamplerState SS_Antialias {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Mirror;
	AddressV = Mirror;
};

float4 PS_Normal(PS_IN input) : SV_Target{
	return Texture.Sample(SS_Antialias, input.uv);
}

float4 PS_Gauss(PS_IN input) : SV_Target {
	//float2 dx, dy;
	//dx = ddx(input.uv);
	//dy = ddy(input.uv);

	//float4 color;
	//color = float4(0, 0, 0, 0);
	//
	//for (int x = -SampleDensity.x; x <= SampleDensity; x++){

	//}

	//for (int x = SampleArea.; x <= area_size; i += interval) {
	//	color += Texture.Sample(SS_Antialias, (input.uv + (dx * j) + (dy * (i - j)))) * d;
	//	div += d;
	//}

	//color /= div;
	//// そもそも加算合成なので、アルファは考慮しなくてよい
	//color.a = 1;

	//return color;

	return float4(0, 0, 0, 0);
}

technique10 Tq_Normal {
	pass NormalPass {
		SetVertexShader(CompileShader(vs_4_0, VS_Default()));
		SetGeometryShader(CompileShader(gs_4_0, GS_Default()));
		SetPixelShader(CompileShader(ps_4_0, PS_Normal()));
	}
};

technique10 Tq_Gaussian {
	pass GaussPass {
		SetVertexShader(CompileShader(vs_4_0, VS_Default()));
		SetGeometryShader(CompileShader(gs_4_0, GS_Default()));
		SetPixelShader(CompileShader(ps_4_0, PS_Gauss()));
	}
};



cbuffer Schene {
	float4x4 View;
	float4x4 Projection;
	float3 LightDirection;
	float3 LightAmbientColor;
	float3 LightDiffuseColor;
	float3 LightSpecularColor;
}

cbuffer Object {
	float4x4 World;
};

cbuffer Face {
	float3 Color;
	float  Ambient;
	float  Diffuse;
	float  Specular;
	float  Emission;
	float  SpecularPower;
	float  Alpha;
};

// TODO: add effect parameters here.

struct Vertex {
	float4 Position : SV_Position;
	float4 Color    : COLOR;
};

struct VS_Output {
	float4 Position : SV_POSITION;
	float4 Color    : COLOR;
};

struct GS_Output {
	float4 Position : SV_POSITION;
	float4 Color    : COLOR;
	float  Factor   : LIGHT;
};

VS_Output VS_Default(Vertex input) {
	VS_Output output;

	float4 worldPosition = mul(input.Position, World);
	float4 viewPosition = mul(worldPosition, View);
	float4 projPosition = mul(viewPosition, Projection);
	output.Position = projPosition;
	output.Color = input.Color;

	return output;
}

[maxvertexcount(3)]
void GS_CreateNormal( triangle VS_Output input[3], inout TriangleStream<GS_Output> stream ) {
	GS_Output output;

	float3 Normal = normalize(cross((input[1].Position - input[0].Position).xyz, (input[2].Position - input[0].Position).xyz));
	float n = mul(-normalize(LightDirection), Normal);
	output.Factor = abs(n + abs(n)) / 2;

	output.Position = input[0].Position;
	output.Color = input[0].Color;
	stream.Append(output);

	output.Position = input[1].Position;
	output.Color = input[1].Color;
	stream.Append(output);

	output.Position = input[2].Position;
	output.Color = input[2].Color;
	stream.Append(output);

	stream.RestartStrip();
}

float4 PS_Lighted(GS_Output input) : SV_TARGET0 {
	float3 ambient = (Ambient * Color) * LightAmbientColor;
	float3 diffuse = (Diffuse * Color) * LightDiffuseColor * input.Factor;
	float3 specular = (Specular * Color) * LightSpecularColor * pow(input.Factor, SpecularPower);
	float3 emission = (Emission * Color);

	return float4(ambient + diffuse + specular + emission, Alpha);
}

float4 PS_Unlighted(VS_Output input) : SV_TARGET {

	float3 ambient = Ambient * Color * LightAmbientColor;
	float3 emissive = Emission * Color;

	return float4(ambient + emissive, Alpha);
}

technique TQ_Lighted {
	pass MainPass {
		SetVertexShader = compile vs_4_0 VS_Default();
		GeometryShader = compile gs_4_0 GS_CreateNormal();
		PixelShader = compile ps_4_0 PS_Lighted();
	}
}

technique TQ_Unlighted {
	pass MainPass {
		VertexShader = compile vs_4_0 VS_Default();
		GeometryShader = 0;
		PixelShader = compile ps_4_0 PS_Unlighted();
	}
}



/*
struct Vertex {
	float4 Position : SV_POSITION;
	float4 Color    : COLOR;
};

float4 VS(Vertex input) : SV_POSITION {
	return input.Position;
}

float4 PS(float4 input : SV_POSITION) : SV_TARGET {
	return float4(0.5, 0.5, 0.5, 0.5);
}



struct VS_IN
{
	float4 pos : POSITION;
	float4 col : COLOR;
};

struct GS_IN
{
	float4 transformed : SV_POSITION;
	float4 pos : POSITION;
	float4 col : COLOR;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

float4x4 worldViewProj;

GS_IN VS( VS_IN input )
{
	GS_IN output = (GS_IN)0;
	
//	output.transformed = mul(input.pos, worldViewProj);
	output.transformed = input.pos;
	output.pos = mul(float4(0, 0, 0, 1), worldViewProj);
	output.col = input.col;

	return output;
}

[maxvertexcount(6)]
void GS( triangle GS_IN input[3], inout TriangleStream<PS_IN> stream )
{
	PS_IN output;
	// (1)
	output.pos = input[0].transformed;
	output.col = input[0].col;
	stream.Append(output);

	output.pos = input[1].transformed;
	output.col = input[1].col;
	stream.Append(output);

	output.pos = input[2].transformed;
	output.col = input[2].col;
	stream.Append(output);

	stream.RestartStrip();

	// (2)
	output.pos = ( input[2].transformed - input[2].pos ) * 1.125 + input[2].pos;
	output.col = float4(1, 1, 1, 1);
	stream.Append(output);

	output.pos = ( input[1].transformed - input[1].pos ) * 1.125 + input[1].pos;
	output.col = float4(1, 1, 1, 1);
	stream.Append(output);

	output.pos = ( input[0].transformed - input[0].pos ) * 1.125 + input[0].pos;
	output.col = float4(1, 1, 1, 1);
	stream.Append(output);

	stream.RestartStrip();

}

float4 PS( PS_IN input ) : SV_Target
{
	return input.col;
	//return float4(1,1,1,0.1);
}

technique TQ_Unlighted {
	pass  {
		VertexShader = compile vs_4_0 VS();
		GeometryShader = compile gs_4_0 GS();
		PixelShader = compile ps_4_0 PS();
	}
}
*/
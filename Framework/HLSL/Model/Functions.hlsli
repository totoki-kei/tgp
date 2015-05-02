#if !defined(HLSL_FUNCTIONS)
#define HLSL_FUNCTIONS

#include "PL_Model.hlsli"

float4 MidairBlend(float4 lower, float4 upper) {
	float4 output;

	output.a = lower.a + upper.a - lower.a * upper.a;
	output.rgb = lower.rgb * lower.a + upper.rgb * upper.a - lower.a * upper.a * lower.rgb;
	output.rgb /= output.a;

	return output;
}

float Edge(float3 edge, float power){
	bool check;
	
	check = edge.x > 1 - power
		 || edge.y > 1 - power
		 || edge.z > 1 - power;

	return check ? 1 : 0;
}

float4x4 GetModelMatrix(uint id){
	uint base = id * 8;
	return transpose(float4x4(
		InstanceBuffer.Load(base),
		InstanceBuffer.Load(base + 1),
		InstanceBuffer.Load(base + 2),
		InstanceBuffer.Load(base + 3)
		));
}

float4 GetModelMaterialInfo(uint id, uint index) {
	uint base = id * 8;
	return InstanceBuffer.Load(base + 4 + index);
}


float4 GetModelColor(uint id, uint index) {
	float4 matinfo;
	float4 color;
	uint mid;

	matinfo = GetModelMaterialInfo(id, index);
	mid = (int)matinfo.x;
	
	color = Material[mid].Color;
	color.a = saturate(color.a * matinfo.a);
	
	return color;
}

float GetModelColorBlend(uint id, uint index) {
	return GetModelMaterialInfo(id, index).y;
}

float GetModelLineWidth(uint id, uint index) {
	return GetModelMaterialInfo(id, index).z;
}



VS_OUT Transform(VS_IN input, uint id : SV_InstanceID) {
	VS_OUT output;

	float4 worldPosition, viewPosition, projPosition;
	float4 worldNormal;
	float4x4 world;

	world = GetModelMatrix(id);

	worldPosition = mul(world, input.position);
	viewPosition = mul(View, worldPosition);
	projPosition = mul(Projection, viewPosition);
	worldNormal = mul(world, float4(input.normal, 0));

	output.position = projPosition;
	output.world = worldPosition.xyz;
	output.normal = normalize(worldNormal.xyz);

	{
		float4 matColor;
		float blend;

		matColor = GetModelColor(id, input.paramIndex);
		blend = GetModelColorBlend(id, input.paramIndex);
		output.color
			= input.color * (1 - blend)
			+ matColor * blend;
	}

	output.width = length(mul(world, float4(GetModelLineWidth(id, input.paramIndex), 0, 0, 1)));

	return output;
}


#endif // HLSL_FUNCTIONS

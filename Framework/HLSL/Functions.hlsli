#if !defined(HLSL_FUNCTIONS)
#define HLSL_FUNCTIONS

float4 MidairBlend(float4 lower, float4 upper) {
	float4 output;

	output.a = lower.a + upper.a - lower.a * upper.a;
	output.rgb = lower.rgb * lower.a + upper.rgb * upper.a - lower.a * upper.a * lower.rgb;
	output.rgb /= output.a;

	return output;
}

float Edge(float4 edge){
	float f;
	float4 fa, fb;

	fa.xyz = edge.xyz + edge.yzx;
	fa.w = edge.w;
	f = max(0, fa.x * fa.x * fa.x);
	f = max(f, fa.y * fa.y * fa.y);
	f = max(f, fa.z * fa.z * fa.z);
	f = max(f, fa.w);

	return f;
}

#endif // HLSL_FUNCTIONS


#include "Common.fx"


cbuffer Scene {
	float4x4 View;
	float4x4 Projection;
}

cbuffer Object {
	float4x4 World;
};


struct VertexPositionColorEmit {
	// ���W
	float4 position : POSITION;
	// �`��F
	float4 color : COLOR;
	// �����F
	float4 emit : EMIT;
};

struct PS_IN {
	// �ϊ��ςݍ��W
	float4 position : SV_Position;
	// �`��F
	float4 color : COLOR;
	// �����F
	float4 emit : EMIT;
};

struct PS_OUT {
	// ����
	float4 color : SV_Target;
};

typedef VertexPositionColorEmit VS_IN;
typedef PS_IN VS_OUT;
VS_OUT VS_Default(VS_IN input, uint index : SV_VertexID) {
	VS_OUT output;

	float4 worldPosition, viewPosition, projPosition;
	worldPosition = mul(input.position, World);
	viewPosition = mul(worldPosition, View);
	projPosition = mul(viewPosition, Projection);

	output.position = projPosition;
	output.color = input.color;
	output.emit = input.emit;

	return output;
}


PS_OUT PS_Normal(PS_IN input){
	PS_OUT output;

	output.color = input.color;
	return output;
}

PS_OUT PS_Emit(PS_IN input){
	PS_OUT output;

	output.color = input.emit;
	return output;
}


technique10 Tech1 {
	pass NormalPass {
		SetVertexShader(CompileShader(vs_4_0, VS_Default()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_Normal()));
	}

	pass EmitPass {
		SetVertexShader(CompileShader(vs_4_0, VS_Default()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_Emit()));
	}

}




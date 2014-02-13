// ���f���`��p�C�v���C�� �w�b�_

cbuffer Scene {
	float4x4 View;
	float4x4 Projection;
}

cbuffer Object {
	float4x4 World;
};


struct VS_IN {
	// ���W
	float4 position : POSITION;
	// �`��F
	float4 color : COLOR;
	// �����F
	float4 emit : EMIT;
};

struct VS_OUT {
	// �ϊ��ςݍ��W
	float4 position : SV_Position;
	// �`��F
	float4 color : COLOR;
	// �����F
	float4 emit : EMIT;
};


typedef VS_OUT PS_IN;


struct PS_OUT {
	// ����
	float4 color : SV_Target;
};

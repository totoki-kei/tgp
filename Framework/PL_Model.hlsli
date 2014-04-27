// ���f���`��p�C�v���C�� �w�b�_

cbuffer Scene : register(b0) {
	float4x4 View;
	float4x4 Projection;
}

cbuffer Object : register(b1) {
	float4x4 World;
};

cbuffer Subset : register(b2) {
	float4 BaseColor;
	float2 EdgeGradient;
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

typedef VS_OUT GS_IN;

struct GS_OUT {
	// �ϊ��ςݍ��W
	float4 position : SV_Position;
	// �`��F
	float4 color : COLOR;
	// �����F
	float4 emit : EMIT;
	// �G�b�W�W��
	float4 edge : EDGE;
};

typedef GS_OUT PS_IN;


struct PS_OUT {
	// ����
	float4 color : SV_Target;
};

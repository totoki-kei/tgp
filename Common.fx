// Note: DirectX10�p�G�t�F�N�g
/*
** ���ʂŎg�p���邩������Ȃ��\����
*/

struct MultiTarget {
	float4 Main : SV_Target0;
	float4 Sub : SV_Target1;
};

/*
** ���ʂŎg�p����֐�
*/

float Color2Lightness(float4 color) {
	float maxVal = max(color.r, max(color.g, color.b));
	float minVal = min(color.r, min(color.g, color.b));
	return (maxVal + minVal) / 2;
}

float4 Color2Lightness4(float4 color) {
	float l = Color2Lightness(color);
	return float4(l, l, l, color.a);
}

MultiTarget SplitHDR(float3 lower, float3 upper, float4 color) {
	MultiTarget m;
	float4 rl = float4(lower, 0);
		float4 rh = float4(upper, 1);
		float4 rr = (color - rl) / (rh.x - rl.x);
		m.Main = saturate(rr);
	m.Sub = float4(saturate(rr.xyz - m.Main.xyz), color.a);
	//m.Sub = float4((rr.xyz - m.Main.xyz), color.a);

	/*
	color |  ~  | lower |  ~      | upper |  ~  | upper * 2 |  ~
	Main  | 0.0 | 0.0   |  /      | 1.0   | 1.0 | 1.0       | 1.0
	Sub   | 0.0 | 0.0   | 0.0     | 0.0   |  /  | 1.0       | 1.0
	*/

	return m;
}

/*
* �e�N�X�`���T���v���[��`
* http://msdn.microsoft.com/ja-jp/library/bb172415(v=vs.85).aspx
*/

SamplerState SS_Default {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

/*
** �����_�����O�X�e�[�g
*/

// *** ���X�^���C�U ***
// D3D10_RASTERIZER_DESC
// http://msdn.microsoft.com/ja-jp/library/bb172408(v=vs.85).aspx

// �ʏ�`��
RasterizerState RS_Normal {
	CullMode = Front;
	FillMode = Solid;
};

// �J�����O�Ȃ�
RasterizerState RS_NoCull {
	CullMode = None;
	FillMode = Solid;
};

// ���C���[�t���[���`��
RasterizerState RS_Wireframe {
	CullMode = None;
	FillMode = Wireframe;
};


// *** �u�����f�B���O ***
// D3D10_BLEND_DESC
// http://msdn.microsoft.com/ja-jp/library/bb204893(v=vs.85).aspx

BlendState BS_Normal {
	BlendEnable[0] = True;
	SrcBlend = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
	BlendOp = Add;
};

BlendState BS_Add {
	BlendEnable[0] = True;
	SrcBlend = Src_Alpha;
	DestBlend = One;
	BlendOp = Add;
};


// *** �[�x�o�b�t�@/�X�e���V�� ***
// D3D10_DEPTH_STENCIL_DESC
// http://msdn.microsoft.com/ja-jp/library/bb205036(v=vs.85).aspx

// ���`�i�V�X�e���f�t�H���g�j
DepthStencilState DSS_Empty {
};

// Depth�Ȃ�
DepthStencilState DSS_NoDepth {
	DepthEnable = false;
};


// Note: DirectX10用エフェクト
/*
** 共通で使用するかもしれない構造体
*/

struct MultiTarget {
	float4 Main : SV_Target0;
	float4 Sub : SV_Target1;
};

/*
** 共通で使用する関数
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
* テクスチャサンプラー定義
* http://msdn.microsoft.com/ja-jp/library/bb172415(v=vs.85).aspx
*/

SamplerState SS_Default {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

/*
** レンダリングステート
*/

// *** ラスタライザ ***
// D3D10_RASTERIZER_DESC
// http://msdn.microsoft.com/ja-jp/library/bb172408(v=vs.85).aspx

// 通常描画
RasterizerState RS_Normal {
	CullMode = Front;
	FillMode = Solid;
};

// カリングなし
RasterizerState RS_NoCull {
	CullMode = None;
	FillMode = Solid;
};

// ワイヤーフレーム描画
RasterizerState RS_Wireframe {
	CullMode = None;
	FillMode = Wireframe;
};


// *** ブレンディング ***
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


// *** 深度バッファ/ステンシル ***
// D3D10_DEPTH_STENCIL_DESC
// http://msdn.microsoft.com/ja-jp/library/bb205036(v=vs.85).aspx

// 空定義（システムデフォルト）
DepthStencilState DSS_Empty {
};

// Depthなし
DepthStencilState DSS_NoDepth {
	DepthEnable = false;
};


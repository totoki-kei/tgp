#if !defined(HLSL_PL_MODEL)
#define HLSL_PL_MODEL

// モデル描画パイプライン ヘッダ


/*
 マクロ定数
*/

//// マルチインスタンス描画における最大インスタンス数
//#define INSTANCE_COUNT 768

// 同時使用マテリアル数
#define MATERIAL_COUNT 32

// ポイントライト数
#define POINTLIGHT_COUNT 16


/*
 定数バッファ構造体
*/

// ポイントライト構造体
struct sPointLight {
	// 点光源のワールド座標
	float3 Position;
	// 点光源の最大到達距離
	float Distance;
	// 点光源の色(Alphaは強さ)
	float4 LightColor;
};

//// インスタンス情報構造体(Bufferを使用するため不要。バッファ使用方法の把握のためにコメントを残す)
//struct sInstance {
//	// ワールド行列
//	float4x4 World;
//	// レンダリングパラメータ(4個セット)
//	// xxx[i].x : マテリアル番号
//	// xxx[i].y : 色割合
//	// xxx[i].z : 線の幅
//	// xxx[i].w : アルファ値係数
//	float4x4 Parameter;
//};

// マテリアル情報構造体
struct sMaterial {
	// 色
	float4 Color;
	//// 色割合(インスタンス情報へ移動)
	//float Blend;
	//// 線の幅(インスタンス情報へ移動)
	//float LineWidth;
};

/*
 定数バッファ
*/

// シーン固有の定数
cbuffer Scene : register(b1) {
	// ビュー行列
	float4x4 View;
	// 射影行列
	float4x4 Projection;

	// 環境ライト
	float4 AmbientColor;
	// 線形ライトの向き
	float3 LightDirection;
	// 線形ライトの色(Alphaは強さ)
	float4 LightColor;

	// ポイントライト
	sPointLight PointLights[POINTLIGHT_COUNT];
}


//// モデル単位の定数
//cbuffer InstanceSet : register(b1) {
//	sInstance Instance[INSTANCE_COUNT];
//};

// マテリアル
cbuffer MaterialSet : register(b2) {
	sMaterial Material[MATERIAL_COUNT];
};

/*
 バッファ
*/

// モデル単位の定数
Buffer<float4> InstanceBuffer;


/*
 パイプライン構造体
*/


//
// 頂点シェーダ
//
struct VS_IN {
	// 座標
	float4 position : POSITION;
	// 描画色
	float4 color : COLOR;
	// 法線
	float3 normal : NORMAL;
	// インスタンスパラメータ番号
	uint paramIndex : PARAM_INDEX;
};

struct VS_OUT {
	// [システム値] 変換済み座標
	float4 position : SV_Position;
	// ワールド座標系の座標
	float3 world : WORLD;
	// ワールド変換済み法線
	float3 normal : NORMAL;
	// 描画色
	float4 color : COLOR;
	// 線の幅
	float width : LINEWIDTH;
};

//
// ジオメトリシェーダ用構造体は定義しない
// (頂点シェーダ出力情報と同じデータ形式)
//

//
// ピクセルシェーダ
//

typedef VS_OUT PS_IN;

struct PS_OUT {
	// 結果
	float4 color : SV_Target;
};

#endif // HLSL_PL_MODEL

#pragma once

// DirectX11の全体のヘッダ等
#pragma warning(push)
#pragma warning(disable:4005)
#include <d3d11.h>
#include <DXGI.h>
//#include <D3DX11.h>
#include <DxErr.h>
#pragma warning(pop)

#include <DirectXMath.h> // DirectXMath本体、floatのベクトルと行列
#include <DirectXColors.h> // 定義済みの色リスト(XMVECTORF32型)
#include <DirectXPackedVector.h> // float以外の型(小型整数の行列とか色とか)も入ったやつ

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace DirectX::Colors;



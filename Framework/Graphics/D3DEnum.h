#pragma once

#include "D3DHeaders.h"

//D3D11_PRIMITIVE_TOPOLOGY
enum class D3DPrimitiveTopology : int {
	Undefined = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED,
	PointList = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	LineList = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	LineStrip = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
	TriangleList = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	TriangleStrip = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	LineListAdj = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
	LineStripAdj = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
	TriangleListAdj = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
	TriangleStripAdj = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,
};

enum class D3DBlendMode : int {
	Normal,
	Alpha,
	Add,
};

template <int Format> struct FormatSize {	/* empty */ };
template <> struct FormatSize < DXGI_FORMAT_R32G32B32A32_TYPELESS > { enum { bits = 128, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32G32B32A32_FLOAT > { enum { bits = 128, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32G32B32A32_UINT > { enum { bits = 128, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32G32B32A32_SINT > { enum { bits = 128, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32G32B32_TYPELESS > { enum { bits = 96, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32G32B32_FLOAT > { enum { bits = 96, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32G32B32_UINT > { enum { bits = 96, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32G32B32_SINT > { enum { bits = 96, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16G16B16A16_TYPELESS > { enum { bits = 64, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16G16B16A16_FLOAT > { enum { bits = 64, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16G16B16A16_UNORM > { enum { bits = 64, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16G16B16A16_UINT > { enum { bits = 64, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16G16B16A16_SNORM > { enum { bits = 64, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16G16B16A16_SINT > { enum { bits = 64, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32G32_TYPELESS > { enum { bits = 64, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32G32_FLOAT > { enum { bits = 64, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32G32_UINT > { enum { bits = 64, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32G32_SINT > { enum { bits = 64, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32G8X24_TYPELESS > { enum { bits = 64, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_D32_FLOAT_S8X24_UINT > { enum { bits = 64, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS > { enum { bits = 64, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_X32_TYPELESS_G8X24_UINT > { enum { bits = 64, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R10G10B10A2_TYPELESS > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R10G10B10A2_UNORM > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R10G10B10A2_UINT > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R11G11B10_FLOAT > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8G8B8A8_TYPELESS > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8G8B8A8_UNORM > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8G8B8A8_UNORM_SRGB > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8G8B8A8_UINT > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8G8B8A8_SNORM > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8G8B8A8_SINT > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16G16_TYPELESS > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16G16_FLOAT > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16G16_UNORM > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16G16_UINT > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16G16_SNORM > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16G16_SINT > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32_TYPELESS > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_D32_FLOAT > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32_FLOAT > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32_UINT > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R32_SINT > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R24G8_TYPELESS > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_D24_UNORM_S8_UINT > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R24_UNORM_X8_TYPELESS > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_X24_TYPELESS_G8_UINT > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8G8_TYPELESS > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8G8_UNORM > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8G8_UINT > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8G8_SNORM > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8G8_SINT > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16_TYPELESS > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16_FLOAT > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_D16_UNORM > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16_UNORM > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16_UINT > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16_SNORM > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R16_SINT > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8_TYPELESS > { enum { bits = 8, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8_UNORM > { enum { bits = 8, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8_UINT > { enum { bits = 8, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8_SNORM > { enum { bits = 8, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8_SINT > { enum { bits = 8, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_A8_UNORM > { enum { bits = 8, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R1_UNORM > { enum { bits = 1, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R9G9B9E5_SHAREDEXP > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_R8G8_B8G8_UNORM > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_G8R8_G8B8_UNORM > { enum { bits = 32, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_B5G6R5_UNORM > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_B5G5R5A1_UNORM > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_B8G8R8A8_UNORM > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };
template <> struct FormatSize < DXGI_FORMAT_B8G8R8X8_UNORM > { enum { bits = 16, bytes = (bits - 1) / 8 + 1, }; };

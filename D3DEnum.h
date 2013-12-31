#pragma once

// DXGIä÷åWÇÃÇ‡ÇÃÇ™çƒíËã`Ç≥ÇÍÇÈÇΩÇﬂÅAàÍïîåxçêÇñ≥å¯Ç…
#pragma warning(push)
#pragma warning(disable:4005)
#include <d3d10.h>
#include <DXGI.h>
#include <D3DX10.h>
#include <DxErr.h>
#pragma warning(pop)

//D3D10_PRIMITIVE_TOPOLOGY
enum class D3DPrimitiveTopology : int {
	Undefined = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
	PointList = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
	LineList = D3D_PRIMITIVE_TOPOLOGY_LINELIST,
	LineStrip = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
	TriangleList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	TriangleStrip = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
	LineListAdj = D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
	LineStripAdj = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
	TriangleListAdj = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
	TriangleStripAdj = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,
};
#pragma once

// DirectX10�̃��C�u�����֌W

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d10.lib")
#if _DEBUG
#pragma comment(lib, "d3dx10d.lib")
#else
#pragma comment(lib, "d3dx10.lib")
#endif
#pragma comment(lib, "dxerr.lib")


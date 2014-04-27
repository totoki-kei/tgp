#pragma once

// Windows.hを読み込むためのヘッダ

#include <Windows.h>

// バッティングするマクロを無効にする
#ifdef FindResource
#undef FindResource
#endif

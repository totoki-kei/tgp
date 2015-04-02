#pragma once

#include <XAudio2.h>
#include <stdint.h>

inline UINT32 CalcSamplePoint(const WAVEFORMATEX &format, UINT32 position, UINT32 dividePerSec) {
	auto ret = (uint64_t)format.nAvgBytesPerSec * position / dividePerSec;

	// nBlockAlignÇ…ÇÊÇÈï‚ê≥
	ret = ((ret + format.nBlockAlign / 2) / format.nBlockAlign) * format.nBlockAlign;

	return (UINT32)ret;
}

#pragma once

#include "SoundData.h"

class WavLoader {
public:
	WavLoader();
	~WavLoader();

	struct Parameter {
		const TCHAR* Filename;
		const BYTE* RiffDataPtr;
		size_t RiffDataLength;

		long DividePerSec;
		long StartPos;
		long EndPos;
	};

	std::vector<BYTE> Load(const Parameter& param, WAVEFORMATEX* outFormat);
};


#include "SoundData.h"

SoundData::SoundData(SoundData&& s)
	: bytes(std::move(s.bytes)), format(s.format) {}

SoundData::SoundData(std::vector<BYTE> &&bytes, const WAVEFORMATEX& format)
	: bytes(std::move(bytes)), format(format) {}

SoundData::~SoundData() {}


#pragma once

#include <XAudio2.h>
#include <memory>
#include <vector>

struct SoundData {
	typedef std::shared_ptr<SoundData> Ptr;
	typedef std::shared_ptr<const SoundData> ConstPtr;

	std::vector<BYTE> bytes;
	WAVEFORMATEX format;

	SoundData() = default;
	SoundData(const SoundData&) = default;
	SoundData(SoundData&&);
	SoundData(std::vector<BYTE> &&bytes, const WAVEFORMATEX& format);

	// �^���� Loader �͈ȉ��̗v���𖞂����K�v������
	// (1) �^ Parameter �����J���Ă���
	// (2) �����o�֐� std::vector<BYTE> Load(Loader::param&, WAVEFORMATEX*) �����J���Ă���
	template <typename Loader>
	static SoundData::Ptr Load(Loader& loader, typename Loader::Parameter &param) {
		SoundData* ret = new SoundData();

		ZeroMemory(&ret->format, sizeof(WAVEFORMATEX));
		ret->bytes = loader.Load(param, &ret->format);
		if (ret->bytes.size() == 0) {
			// �ǂݍ��݂Ɏ��s����
			delete ret;
		}

		return SoundData::Ptr(ret);
	}

	~SoundData();
};

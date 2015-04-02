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

	// 型引数 Loader は以下の要件を満たす必要がある
	// (1) 型 Parameter を公開している
	// (2) メンバ関数 std::vector<BYTE> Load(Loader::param&, WAVEFORMATEX*) を公開している
	template <typename Loader>
	static SoundData::Ptr Load(Loader& loader, typename Loader::Parameter &param) {
		SoundData* ret = new SoundData();

		ZeroMemory(&ret->format, sizeof(WAVEFORMATEX));
		ret->bytes = loader.Load(param, &ret->format);
		if (ret->bytes.size() == 0) {
			// 読み込みに失敗した
			delete ret;
		}

		return SoundData::Ptr(ret);
	}

	~SoundData();
};

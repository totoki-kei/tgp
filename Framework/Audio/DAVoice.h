#pragma once

class DAVoice;

#include "DACore.h"
#include "SoundData.h"
#include "DAVoiceCallback.h"

class DAVoice : public Resource {
protected:
	DACore* core;
	IXAudio2SourceVoice* sv;
	DAVoiceCallback::Ptr cb;
	WAVEFORMATEX format;
	bool playing;

public:
	struct Loops {
		UINT32 DividePerSec;
		UINT32 StartPos;
		UINT32 EndPos;
		UINT32 LoopStartPos;
		UINT32 LoopEndPos;
		UINT32 LoopCount;

		Loops();
		Loops(UINT32 count);
	};

	struct Block {
		SoundData::ConstPtr Data;
		bool LoopEnable;
		Loops LoopPoint;

		Block() = delete;
		Block(const Block&);
		Block(Block&&);
		Block(SoundData::ConstPtr, bool, const Loops&);
	};

private:

	std::vector<Block> chain;

public:


	DAVoice() = delete;

	DAVoice(const DAVoice&);
	DAVoice(DAVoice&&);

	DAVoice(DACore* core, WAVEFORMATEX& format, DAVoiceCallback::Ptr callback = nullptr);

	~DAVoice();

	void Play();
	void Next();
	void Stop();

	void PushSoundData(SoundData::ConstPtr data);
	void PushSoundData(SoundData::ConstPtr data, const Loops& loop);
	void PushSoundData(SoundData::ConstPtr data, bool loop);

	void SetVolume(float);
	float GetVolume() const;
	void SetFrequency(float);
	float GetFrequency() const;

};

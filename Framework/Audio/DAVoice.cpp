#include "DAVoice.h"
#include "DAUtility.h"

#include <XAudio2.h>

DAVoice::Loops::Loops() {
	DividePerSec = 1;
	StartPos = EndPos = LoopStartPos = LoopEndPos = 0;
	LoopCount = 0;
}

DAVoice::Loops::Loops(UINT32 n) {
	DividePerSec = 1;
	StartPos = EndPos = LoopStartPos = LoopEndPos = 0;
	LoopCount = n;
}



DAVoice::Block::Block(const DAVoice::Block& b)
	: Data(b.Data)
	, LoopEnable(b.LoopEnable)
	, LoopPoint(b.LoopPoint) {}

DAVoice::Block::Block(DAVoice::Block&& b)
	: Data(std::move(b.Data))
	, LoopEnable(b.LoopEnable)
	, LoopPoint(b.LoopPoint) {}

DAVoice::Block::Block(SoundData::ConstPtr d, bool lf, const Loops& l)
	: Data(d)
	, LoopEnable(lf)
	, LoopPoint(l) {}

DAVoice::DAVoice(DACore* core, WAVEFORMATEX& format, DAVoiceCallback::Ptr callback) {
	this->core = core;
	this->cb = callback;
	this->format = format;

	switch (HRESULT hr = core->GetDevice()->CreateSourceVoice(&sv, &format, 0U, 2.0f, cb.get())) {
		case S_OK:
			break;
		default:
			break;
	}


}

DAVoice::DAVoice(const DAVoice &v) 
	: core(v.core)
	, cb(v.cb)
	, chain(v.chain)
	, format(v.format)
{


	switch (HRESULT hr = core->GetDevice()->CreateSourceVoice(&sv, &format, 0U, 2.0f, cb.get())) {
		case S_OK:
			break;
		default:
			break;
	}

}

DAVoice::DAVoice(DAVoice&& v)
	: core(v.core)
	, sv(v.sv) 
	, cb(std::move(v.cb)) 
	, chain(std::move(v.chain))
	, format(v.format) 
{
	v.sv = nullptr;
}


DAVoice::~DAVoice() {
	if (sv) {
		sv->Stop();
		sv->DestroyVoice();
	}
}

void DAVoice::Play() {
	if (playing) {
		Stop();
	}
	playing = true;

	
	for (auto it = chain.begin(); it != chain.end(); it++) {
		auto& block = *it;

		XAUDIO2_BUFFER buffer = { 0 };
		buffer.pAudioData = block.Data->bytes.data();
		buffer.AudioBytes = block.Data->bytes.size();
		buffer.Flags = (it + 1 == chain.end() ? XAUDIO2_END_OF_STREAM : 0);
		if (block.LoopEnable) {
			const Loops* loop = &block.LoopPoint;
			buffer.PlayBegin = CalcSamplePoint(format, loop->StartPos, loop->DividePerSec);
			buffer.PlayLength = CalcSamplePoint(format, loop->EndPos, loop->DividePerSec) - buffer.PlayBegin;
			buffer.LoopBegin = CalcSamplePoint(format, loop->LoopStartPos, loop->DividePerSec);
			buffer.LoopLength = CalcSamplePoint(format, loop->LoopEndPos, loop->DividePerSec) - buffer.LoopBegin;
			buffer.LoopCount = loop->LoopCount;
		}
		buffer.pContext = (void*)block.Data.get();

		sv->SubmitSourceBuffer(&buffer);
	}

	sv->Start();

	
}

void DAVoice::Next() {
	if (playing) {
		sv->ExitLoop();
	}
}

void DAVoice::Stop() {
	sv->Stop();
	sv->FlushSourceBuffers();

	playing = false;
}

void DAVoice::PushSoundData(SoundData::ConstPtr data) {
	chain.emplace_back(data, false, Loops{});
}
void DAVoice::PushSoundData(SoundData::ConstPtr data, const Loops& loop) {
	chain.emplace_back(data, true, loop);
}
void DAVoice::PushSoundData(SoundData::ConstPtr data, bool loop) {
	Loops l = { 0 };
	l.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
	chain.emplace_back(data, loop, l);
}


void DAVoice::SetVolume(float v) {
	sv->SetVolume(v);
}

float DAVoice::GetVolume() const {
	float ret;
	sv->GetVolume(&ret);
	return ret;
}

void DAVoice::SetFrequency(float f) {
	sv->SetFrequencyRatio(f);
}

float DAVoice::GetFrequency() const {
	float ret;
	sv->GetFrequencyRatio(&ret);
	return ret;
}

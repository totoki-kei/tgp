#pragma once

class DACore;

#include "../Resource.h"

#include <memory>
#include <XAudio2.h>


class DACore : public Resource {

	bool coinitialized;

	IXAudio2* ixa;
	IXAudio2MasteringVoice* mstv;

public:
	DACore();
	~DACore();

	IXAudio2* GetDevice() const;
	IXAudio2MasteringVoice* GetMasteringVoice() const;


	
};


#include "DACore.h"

#include "../Debug.h"

DACore::DACore() : ixa{ nullptr }, mstv{ nullptr } {
	switch (HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED)) {
		case S_OK:
			coinitialized = true;
			break;
		case S_FALSE:
			coinitialized = false;
			break;
		default:
			LOG_ERR("failed execute CoInitializeEx (ret = 0x%08x)\n", hr);
			return;
	}

#if _DEBUG
	int flag = XAUDIO2_DEBUG_ENGINE;
#else
	int flag = 0;
#endif
	switch (HRESULT hr = XAudio2Create(&ixa, flag)) {
		case S_OK:
			// ok
			break;
		default:
			LOG_ERR("failed execute XAudio2Create (ret = 0x%08x)\n", hr);
			if (coinitialized) {
				CoUninitialize();
				coinitialized = false;
			}
			return;
	}

	switch (HRESULT hr = ixa->CreateMasteringVoice(&mstv)) {
		case S_OK:
			// ok
			break;
		default:
			LOG_ERR("failed to create MasteringVoice (ret = 0x%08x)\n", hr);
			return;
	}
}

DACore::~DACore() {
	mstv->DestroyVoice();
	mstv = nullptr;
	ixa->Release();
	ixa = nullptr;

	if (coinitialized) {
		CoUninitialize();
	}
}

IXAudio2* DACore::GetDevice() const {
	return ixa;
}

IXAudio2MasteringVoice* DACore::GetMasteringVoice() const {
	return mstv;
}

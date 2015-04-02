#pragma once

#include <XAudio2.h>
#include <memory>

class DAVoiceCallback : public IXAudio2VoiceCallback {
public:
	typedef std::shared_ptr<DAVoiceCallback> Ptr;
	typedef std::shared_ptr<const DAVoiceCallback> ConstPtr;

	virtual ~DAVoiceCallback() {}

	// ボイスが連続オーディオ ストリームの再生を終了したときに呼び出されます。
	void _stdcall OnStreamEnd() override {}

	// ボイス終了のプロセッシング パスの直後に呼び出されます。
	void _stdcall OnVoiceProcessingPassEnd() override {}

	// ボイス開始のプロセッシング パスの直前に呼び出されます。
	// [引数] UINT32 : すぐに必要なデータ量(バイト)
	void _stdcall OnVoiceProcessingPassStart(UINT32) override {}
	
	// ボイスがバッファーの処理を終了したときに呼び出されます。
	// [引数] void* : Submit時のバッファに設定された追加情報
	void _stdcall OnBufferEnd(void*) override {}

	// ボイスが新しいオーディオ バッファーの処理を開始しようとするときに呼び出されます。
	// [引数] void* : Submit時のバッファに設定された追加情報
	void _stdcall OnBufferStart(void*) override {}

	// ボイスがループの最後に到達したときに呼び出されます。
	// [引数] void* : Submit時のバッファに設定された追加情報
	void _stdcall OnLoopEnd(void*) override {}
	
	// ボイスの処理中に重大なエラーが発生したときに呼び出されます。
	// [引数] void* : Submit時のバッファに設定された追加情報
	// [引数] HRESULT : エラーコード(HRESULT型)
	void _stdcall OnVoiceError(void*, HRESULT) override {}

};
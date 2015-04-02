#pragma once

#include <XAudio2.h>
#include <memory>

class DAVoiceCallback : public IXAudio2VoiceCallback {
public:
	typedef std::shared_ptr<DAVoiceCallback> Ptr;
	typedef std::shared_ptr<const DAVoiceCallback> ConstPtr;

	virtual ~DAVoiceCallback() {}

	// �{�C�X���A���I�[�f�B�I �X�g���[���̍Đ����I�������Ƃ��ɌĂяo����܂��B
	void _stdcall OnStreamEnd() override {}

	// �{�C�X�I���̃v���Z�b�V���O �p�X�̒���ɌĂяo����܂��B
	void _stdcall OnVoiceProcessingPassEnd() override {}

	// �{�C�X�J�n�̃v���Z�b�V���O �p�X�̒��O�ɌĂяo����܂��B
	// [����] UINT32 : �����ɕK�v�ȃf�[�^��(�o�C�g)
	void _stdcall OnVoiceProcessingPassStart(UINT32) override {}
	
	// �{�C�X���o�b�t�@�[�̏������I�������Ƃ��ɌĂяo����܂��B
	// [����] void* : Submit���̃o�b�t�@�ɐݒ肳�ꂽ�ǉ����
	void _stdcall OnBufferEnd(void*) override {}

	// �{�C�X���V�����I�[�f�B�I �o�b�t�@�[�̏������J�n���悤�Ƃ���Ƃ��ɌĂяo����܂��B
	// [����] void* : Submit���̃o�b�t�@�ɐݒ肳�ꂽ�ǉ����
	void _stdcall OnBufferStart(void*) override {}

	// �{�C�X�����[�v�̍Ō�ɓ��B�����Ƃ��ɌĂяo����܂��B
	// [����] void* : Submit���̃o�b�t�@�ɐݒ肳�ꂽ�ǉ����
	void _stdcall OnLoopEnd(void*) override {}
	
	// �{�C�X�̏������ɏd��ȃG���[�����������Ƃ��ɌĂяo����܂��B
	// [����] void* : Submit���̃o�b�t�@�ɐݒ肳�ꂽ�ǉ����
	// [����] HRESULT : �G���[�R�[�h(HRESULT�^)
	void _stdcall OnVoiceError(void*, HRESULT) override {}

};
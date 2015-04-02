#pragma once
#define INTERNAL

#include "Stdafx.h"
#include "tgp_lib.h"

using namespace System;

namespace TgpLib {
	namespace Native = tgp_lib;

	public ref class Config {
#pragma region GameInput 
	public:
		ref class GameInput {
		internal:
			Native::Config::GameInput *ptr;

		public:
			property bool UseDigitalPad {
				bool get() { return ptr->useDigitalPad; }
				void set(bool value) { ptr->useDigitalPad = value; }
			}
			property UInt32 Action{
				UInt32 get() { return ptr->action; }
				void set(UInt32 value) { ptr->action = value; }
			}
			property UInt32 Subaction{
				UInt32 get() { return ptr->subaction; }
				void set(UInt32 value) { ptr->subaction = value; }
			}
			property UInt32 RotateLeft{
				UInt32 get() { return ptr->rotateLeft; }
				void set(UInt32 value) { ptr->rotateLeft = value; }
			}
			property UInt32 RotateRight{
				UInt32 get() { return ptr->rotateRight; }
				void set(UInt32 value) { ptr->rotateRight = value; }
			}
			property UInt32 Menu{
				UInt32 get() { return ptr->menu; }
				void set(UInt32 value) { ptr->menu = value; }
			}
		};
#pragma endregion
	private:

		Native::Config* ptr;
		GameInput keyboard;
		GameInput gamepad;

	public :
		Config() {
			ptr = new Native::Config();
			Native::InitConfig(ptr);

			keyboard.ptr = &ptr->keyboard;
			gamepad.ptr = &ptr->gamepad;
		}

		~Config() {
			this->!Config();
		}
		!Config(){
			delete ptr;
		}

		void ThrowExceptionFromErrno(errno_t e, String^ message){
			String^ text = String::Format("{0} (errno: {1})", message, e);
			switch (e){
			case ENOENT: // �Y������t�@�C���܂��̓f�B���N�g��������܂���
				throw gcnew System::IO::FileNotFoundException(text);
			case EIO: // I/O �G���[�ł�
				throw gcnew System::IO::IOException(text);
				/*
			case EPERM: // ����͋�����Ă��܂���
			case ESRCH: // �Y������v���Z�X������܂���
			case EINTR: // �֐��Ɋ��荞�݂�����܂���
			case ENXIO: // �Y������f�o�C�X�܂��̓A�h���X������܂���
			case E2BIG: // �������������܂�
			case ENOEXEC: // ���s�`���G���[�ł�
			case EBADF: // �t�@�C���ԍ�������������܂���
			case ECHILD: // �q�v���Z�X�����݂��܂���
			case EAGAIN: // ����ȏ�v���Z�X�𐶐��ł��Ȃ����A���������s�����Ă��邩�A�ő�̓���q���x���ɒB���܂���
			case ENOMEM: // ���������s�����Ă��܂�
			case EACCES: // �A�N�Z�X���͋��ۂ���܂���
			case EFAULT: // �A�h���X������������܂���
			case EBUSY: // �f�o�C�X�܂��̓��\�[�X���r�W�[��Ԃł�
			case EEXIST: // �t�@�C�������݂��܂�
			case EXDEV: // �N���X �f�o�C�X �����N������܂�
			case ENODEV: // �Y������f�o�C�X������܂���
			case ENOTDIR: // �f�B���N�g���ł͂���܂���
			case EISDIR: // �f�B���N�g���ł�
			case EINVAL: // �����������ł�
			case ENFILE: // �V�X�e���ŊJ���Ă���t�@�C�����������܂�
			case EMFILE: // �J���Ă���t�@�C�����������܂�
			case ENOTTY: // �s�K�؂� I/O ���䑀��ł�
			case EFBIG: // �t�@�C�����傫�����܂�
			case ENOSPC: // �f�o�C�X�ɋ󂫗̈悪����܂���
			case ESPIPE: // �V�[�N�������ł�
			case EROFS: // �ǂݎ���p�t�@�C�� �V�X�e�����g�p����Ă��܂�
			case EMLINK: // �����N���������܂�
			case EPIPE: // �p�C�v���j�󂳂�Ă��܂�
			case EDOM: // �Z�p���Z�������g�p����܂���
			case ERANGE: // ���ʂ��傫�����܂�
			case EDEADLK: // ���\�[�X �f�b�h���b�N���������܂�
			case ENAMETOOLONG: // �t�@�C�������������܂�
			case ENOLCK: // ���b�N������܂���
			case ENOSYS: // �֐��̓T�|�[�g����Ă��܂���
			case ENOTEMPTY: // �f�B���N�g������ł͂���܂���
			case EILSEQ: // �����ȃo�C�g �V�[�P���X�ł�
			case STRUNCATE: // �c��̕�����͐؂�l�߂��܂���
				*/
			default:
				throw gcnew ApplicationException(text);
			}
		}

		void Load(String^ filepath){
			auto wcpath = filepath->ToCharArray();
			pin_ptr<wchar_t> p = &wcpath[0];
			if (errno_t e = Native::LoadConfig(this->ptr, (const wchar_t*)p)){
				ThrowExceptionFromErrno(e, "�ݒ�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B");
			}
		}
		void Save(String^ filepath){
			auto wcpath = filepath->ToCharArray();
			pin_ptr<wchar_t> p = &wcpath[0];
			if (errno_t e = Native::SaveConfig(this->ptr, (const wchar_t*)p)){
				ThrowExceptionFromErrno(e, "�ݒ�t�@�C���̏������݂Ɏ��s���܂����B");
			}
		}

		property bool FullScreen {
			bool get(){ return ptr->fullscreen; }
			void set(bool value) { ptr->fullscreen = value; }
		}

		property UInt32 ScreenWidth {
			UInt32 get() { return ptr->screenWidth; }
			void set(UInt32 value) { ptr->screenWidth = value; }
		}
		property UInt32 ScreenHeight {
			UInt32 get() { return ptr->screenHeight; }
			void set(UInt32 value) { ptr->screenHeight = value; }
		}
		property GameInput^ Keyboard{
			GameInput^ get() { return %keyboard; }
		}
		property GameInput^ Gamepad {
			GameInput^ get() { return %gamepad; }
		}
	};
}


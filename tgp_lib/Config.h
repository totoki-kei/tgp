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
			case ENOENT: // 該当するファイルまたはディレクトリがありません
				throw gcnew System::IO::FileNotFoundException(text);
			case EIO: // I/O エラーです
				throw gcnew System::IO::IOException(text);
				/*
			case EPERM: // 操作は許可されていません
			case ESRCH: // 該当するプロセスがありません
			case EINTR: // 関数に割り込みがありました
			case ENXIO: // 該当するデバイスまたはアドレスがありません
			case E2BIG: // 引数が長すぎます
			case ENOEXEC: // 実行形式エラーです
			case EBADF: // ファイル番号が正しくありません
			case ECHILD: // 子プロセスが存在しません
			case EAGAIN: // これ以上プロセスを生成できないか、メモリが不足しているか、最大の入れ子レベルに達しました
			case ENOMEM: // メモリが不足しています
			case EACCES: // アクセス許可は拒否されました
			case EFAULT: // アドレスが正しくありません
			case EBUSY: // デバイスまたはリソースがビジー状態です
			case EEXIST: // ファイルが存在します
			case EXDEV: // クロス デバイス リンクがあります
			case ENODEV: // 該当するデバイスがありません
			case ENOTDIR: // ディレクトリではありません
			case EISDIR: // ディレクトリです
			case EINVAL: // 引数が無効です
			case ENFILE: // システムで開いているファイルが多すぎます
			case EMFILE: // 開いているファイルが多すぎます
			case ENOTTY: // 不適切な I/O 制御操作です
			case EFBIG: // ファイルが大きすぎます
			case ENOSPC: // デバイスに空き領域がありません
			case ESPIPE: // シークが無効です
			case EROFS: // 読み取り専用ファイル システムが使用されています
			case EMLINK: // リンクが多すぎます
			case EPIPE: // パイプが破壊されています
			case EDOM: // 算術演算引数が使用されました
			case ERANGE: // 結果が大きすぎます
			case EDEADLK: // リソース デッドロックが発生します
			case ENAMETOOLONG: // ファイル名が長すぎます
			case ENOLCK: // ロックがありません
			case ENOSYS: // 関数はサポートされていません
			case ENOTEMPTY: // ディレクトリが空ではありません
			case EILSEQ: // 無効なバイト シーケンスです
			case STRUNCATE: // 残りの文字列は切り詰められました
				*/
			default:
				throw gcnew ApplicationException(text);
			}
		}

		void Load(String^ filepath){
			auto wcpath = filepath->ToCharArray();
			pin_ptr<wchar_t> p = &wcpath[0];
			if (errno_t e = Native::LoadConfig(this->ptr, (const wchar_t*)p)){
				ThrowExceptionFromErrno(e, "設定ファイルの読み込みに失敗しました。");
			}
		}
		void Save(String^ filepath){
			auto wcpath = filepath->ToCharArray();
			pin_ptr<wchar_t> p = &wcpath[0];
			if (errno_t e = Native::SaveConfig(this->ptr, (const wchar_t*)p)){
				ThrowExceptionFromErrno(e, "設定ファイルの書き込みに失敗しました。");
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


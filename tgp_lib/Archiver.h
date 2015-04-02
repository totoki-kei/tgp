#pragma once
#define INTERNAL

#include "Stdafx.h"
#include "tgp_lib.h"

using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::IO::Compression;

namespace TgpLib {
	namespace Native = tgp_lib;

	ref class Archiver {
	public:
		static array<Byte>^ Load(String^ filename) {
			FileStream fs(filename, FileMode::Open);
			GZipStream gz(%fs, CompressionMode::Decompress);

			auto list = gcnew List< Byte >();
			int totalLength = 0;
			
			do {
				array<Byte>^ arr = gcnew array<Byte>(8192);
				int count = gz.Read(arr, 0, 8192);

				if (count == 0) break;

				if (count != 8192) {
					array<Byte>^ insArr = gcnew array<Byte>(count);
					Array::Copy(arr, insArr, count);
					list->AddRange(insArr);
				}
				else {
					list->AddRange(arr);
				}

				totalLength += count;

			} while (true);

			return list->ToArray();
		}

		static void Save(String^ filename, array<Byte>^ data) {
			FileStream fs(filename, FileMode::OpenOrCreate);
			GZipStream gz(%fs, CompressionMode::Compress);

			gz.Write(data, 0, data->Length);
		}
	};
}

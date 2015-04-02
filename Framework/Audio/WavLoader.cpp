#include "WavLoader.h"
#include "DAUtility.h"

#include <stdio.h>
#include <tchar.h>

#include <mmsystem.h>

#include <memory>

#include "../Debug.h"
#include "../Utility.h"

using std::unique_ptr;

WavLoader::WavLoader() {}
WavLoader::~WavLoader() {}

std::vector<BYTE> WavLoader::Load(const Parameter& param, WAVEFORMATEX* outFormat) {
	// ロジックは、 http://marupeke296.com/DS_No2_GetSoundDataFromWav.html をほぼ丸写し。

	ScopeExit onexit;

	HMMIO file;

	// ファイル(またはメモリ)のオープン
	if (!param.Filename) {
		if (!param.RiffDataPtr || param.RiffDataLength <= 0) {
			LOG_ERR("parameter error. both filename and riffData[Length] is not set.");
			return std::vector<BYTE>();
		}

		MMIOINFO info = { 0 };
		info.pchBuffer = (HPSTR)param.RiffDataPtr;
		info.fccIOProc = FOURCC_MEM;
		info.cchBuffer = param.RiffDataLength;

		file = mmioOpen(nullptr, &info, MMIO_READ);

		if (!file) {
			LOG_ERR("cannot open memory 0x%08x (%d bytes)\n", param.RiffDataPtr, param.RiffDataLength);
			return std::vector<BYTE>();
		}
	}
	else {
		auto fnlen = _tcslen(param.Filename) + 1;
		std::unique_ptr<TCHAR[]>fn(new TCHAR[fnlen]);
		_tcscpy_s(fn.get(), fnlen, param.Filename);

		file = mmioOpen(fn.get(), nullptr, MMIO_READ);

		if (!file) {
			LOG_ERR("cannot open file %s\n", param.Filename);
			return std::vector<BYTE>();
		}
	}


	onexit[[&]() {mmioClose(file, 0); }];

	// RIFFチャンク検索
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(file, &riffChunk, NULL, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR) {
		LOG_ERR("invalid file format %s : cannot find 'WAVE' Chunk.\n", param.Filename);
		return std::vector<BYTE>();
	}

	// フォーマットチャンク検索
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(file, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		LOG_ERR("invalid file format %s\n", param.Filename);
		return std::vector<BYTE>();
	}
	// フォーマットチャンク読み出し
	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead(file, (HPSTR)outFormat, fmsize);
	if (size != fmsize) {
		LOG_ERR("invalid file format %s : cannot find 'fmt ' Chunk.\n", param.Filename);
		return std::vector<BYTE>();
	}

	mmioAscend(file, &formatChunk, 0);
	// データチャンク検索
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(file, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR) {
		LOG_ERR("invalid file format %s : cannot find 'data' Chunk.\n", param.Filename);
		return std::vector<BYTE>();
	}

	UINT32 pos;
	UINT32 bytes;
	if (param.DividePerSec > 0 && param.StartPos >= 0 && param.EndPos > 0 && param.StartPos < param.EndPos) {
		pos = CalcSamplePoint(*outFormat, param.StartPos, param.DividePerSec);
		if (pos > dataChunk.cksize) pos = dataChunk.cksize;

		bytes = CalcSamplePoint(*outFormat, param.EndPos, param.DividePerSec);
		if (bytes > dataChunk.cksize) bytes = dataChunk.cksize;

		bytes -= pos;
	}
	else {
		pos = 0;
		bytes = dataChunk.cksize;
	}
	char *pData = new char[bytes];
	onexit[[&]() {delete[] pData; }];

	mmioSeek(file, pos, SEEK_CUR);
	size = mmioRead(file, (HPSTR)pData, bytes);
	//if (size != dataChunk.cksize) {
	//	LOG_ERR("invalid file format %s\n", param.filename);
	//	return std::vector<BYTE>();
	//}
	std::vector<BYTE> ret;
	ret.assign(pData, pData + size);
	return ret;


}





#pragma once

#include <string>
#include <map>
#include <mutex>

#include "Framework/Graphics/Model.h"

class ModelBank {
	typedef std::basic_string<TCHAR> tstring;

	std::mutex mtx;

public:
	struct Setting {
		tstring filename;
		bool solid;
		bool wire;

		bool operator == (const Setting& s) const ;
		bool operator < (const Setting& s) const ;
	};

	std::map<Setting, std::unique_ptr<Models::Model> > cache;

	Models::Model* Get(const TCHAR* filename, bool solid = true, bool wire = true);
	Models::Model* Get(const Setting&);
};

extern ModelBank* g_ModelBank;

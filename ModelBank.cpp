#include "ModelBank.h"

bool ModelBank::Setting::operator ==(const Setting& s) const {
	return filename == s.filename
		&& solid == s.solid
		&& wire == s.wire;
}

bool ModelBank::Setting::operator < (const Setting& s) const {
	int fn = filename.compare(s.filename);
	if (fn == 0) {
		int a = (solid ? 2 : 0) | (wire ? 1 : 0);
		int b = (s.solid ? 2 : 0) | (s.wire ? 2 : 0);
		return a < b;
	}
	else {
		return fn < 0;
	}
}

Models::Model* ModelBank::Get(const TCHAR* filename, bool solid, bool wire) {
	Setting request = { filename, solid, wire };

	return Get(request);
}

Models::Model* ModelBank::Get(const ModelBank::Setting& setting) {
	std::lock_guard<std::mutex> lock(mtx);

	auto it = cache.find(setting);
	if (it != cache.end()) {
		// Œ©‚Â‚©‚Á‚½
		return it->second.get();
	}
	else {
		auto m = Models::Model::Load(setting.filename.c_str(), setting.solid, setting.wire);

		if (!m) {
			LOG_ERR("failed to load modelfile %s", setting.filename.c_str());
		}

		m->SetMaterial(0, Models::MaterialData(XMFLOAT4{ 1.00, 1.00, 1.00, 1.00 }));
		m->SetMaterial(1, Models::MaterialData(XMFLOAT4{ 1.00, 0.25, 0.25, 1.00 }));
		m->SetMaterial(2, Models::MaterialData(XMFLOAT4{ 0.25, 1.00, 0.25, 1.00 }));
		m->SetMaterial(3, Models::MaterialData(XMFLOAT4{ 0.25, 0.25, 1.00, 1.00 }));
		m->SetMaterial(4, Models::MaterialData(XMFLOAT4{ 1.00, 1.00, 0.25, 1.00 }));
		m->SetMaterial(5, Models::MaterialData(XMFLOAT4{ 1.00, 0.25, 1.00, 1.00 }));
		m->SetMaterial(6, Models::MaterialData(XMFLOAT4{ 0.25, 1.00, 1.00, 1.00 }));
		m->SetMaterial(7, Models::MaterialData(XMFLOAT4{ 0.25, 0.25, 0.25, 1.00 }));

		cache.insert({ setting, std::unique_ptr<Models::Model>(m) });
		return m;
	}
}

ModelBank* g_ModelBank;

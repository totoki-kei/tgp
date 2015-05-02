#include "Item.h"
#include "Particle.h"

std::vector<Item> Item::List;
Models::Model* Item::model;

Item::Item() : enabled{ false }, count{ 0 } {
	if(!model) model = g_ModelBank->Get(_T("Content\\Model\\s01.txt"));
}


Item::~Item() {}


Item& Item::Create() {
	return Create(GetRandomPointOnSurface(SURF_NONE));
}
Item& Item::Create(Surface s) {
	return Create(GetRandomPointOnSurface(s));
}
Item& Item::Create(float x, float y, float z) {
	return Create(XMFLOAT3(x, y, z));
}
Item& Item::Create(const XMFLOAT3& p) {
	auto& item = CreateItemImpl();
	item.Init(p);
	return item;
}

Item& Item::CreateItemImpl() {
	
	auto iter = std::find_if(List.begin(), List.end(), [](const Item& i) { return !i.enabled; });
	if (iter != List.end()) {
		return *iter;
	}
	else {
		List.emplace_back();
		return List.back();
	}

}

void Item::Init(const XMFLOAT3& p) {
	//if (
	//	x != 1f && y != 1f && z != 1f &&
	//	x != -1f && y != -1f && z != -1f)
	//	throw new ArgumentException("座標が無効です。");

	this->pos = p;
	enabled = true;

	auto rnd = []() -> float { return GameImpl::GetInstance()->GetRand(-1, 1); };
	XMVECTOR a = XMVector3Normalize(XMVectorSet(rnd(), rnd(), rnd(), 0.0f));
	axis = XMFLOAT3(XMVectorGetX(a), XMVectorGetY(a), XMVectorGetZ(a));

	count = 0;

}

void Item::Update() {
	count++;
}
void Item::Draw() {

	if (model) {
		Models::InstanceData inst;
		XMVECTOR rotaxis = XMVectorSet(axis.x, axis.y, axis.z, 1.0f);
		inst.World = XMMatrixScaling(0.05f, 0.05f, 0.05f)
			* XMMatrixRotationAxis(rotaxis, XM_2PI * (count % 128) / 128.0f)
			* XMMatrixTranslation(pos.x, pos.y, pos.z);

		model->Draw(inst);
	}
}

void Item::UpdateAll() {
	activeCount = 0;
	for (auto& item : List) {
		if (item.enabled) {
			item.Update();
		}
		if (item.enabled) {
			activeCount++;
		}
	}
}

void Item::DrawAll() {
	bool flushFlag = false;
	for (auto& item : List) {
		if (item.enabled) {
			item.Draw();
			flushFlag = true;
		}
	}
	if (flushFlag) model->Flush();
}

int Item::activeCount;
int Item::GetCount() {
	return activeCount;
}

void Item::Vanish() {
	enabled = false;

	// TODO: ここにエフェクトを生成するコードを入れる
	Particle::Generate(96, this->pos, XMFLOAT3{0, 0, 0}, XMFLOAT3{ 1.0f / 32, 1.0f / 32, 1.0f / 32 }, PlayerConsts::ItemCrushArea / 13.69188058f, 4);
}

void Item::Clear() {
	for (auto& item : List) {
		item.enabled = false;
	}
	activeCount = 0;
}

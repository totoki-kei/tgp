#include "Scene.h"


Scene::Scene() : parent{ nullptr }, child{ nullptr } {}
Scene::Scene(Scene* p) : parent{ p }, child{ nullptr } {}

// *** 純粋仮想関数を持つクラスのインスタンスはムーブできない
//Scene::Scene(Scene&& s) : parent{ s.parent }, child{ s.child } {
//	// ムーブコンストラクタは必要だろうか？
//	// 一応定義する。
//	s.parent = nullptr;
//	s.child = nullptr;
//	if (parent) parent->child = this;
//	if (child) child->parent = this;
//}

Scene::~Scene() {
	if (child) delete child;
}


Scene* Scene::Update() {
	Scene* ret;

	if (child) {
		// サブシーンがある場合はそれを実行
		bool pop = false;
		auto s = child->Update();

		if (s == child) {
			// 戻り値がサブシーンそのもの -> 何もしない(引き続きこのサブシーンを使用する)
			ret = this;
		}
		else if(s == this) {
			// 戻り値が自分 -> サブシーン終了
			// サブシーンからこのシーンへの値の受け渡しは
			// これ以前にサブシーン側にて行われたものとする。
			delete child;
			child = nullptr;
			// 自分自身を返して、このシーンを維持させつつ終了
			ret = this;
		}
		else if (s == nullptr) {
			// 戻り値がnullptr -> サブシーン終了
			// thisのケースと異なり、自分自身のシーンを即座に実行する
			delete child;
			child = nullptr;
			// シーンのアップデートを実行しその結果を返す
			ret = this->UpdateScene();
		}
		else {
			// 戻り値がそれら以外のシーン -> サブシーン差し替え
			delete child;
			child = s;
			// 自分自身を返して、このシーンを維持させつつ終了
			ret = this;
		}

	}
	else {
		// サブシーンが無い場合は自分を実行
		ret = this->UpdateScene();
	}

	return ret;
}

Scene* Scene::UpdateScene() {
	return nullptr;
}

void Scene::DrawScene() {
	/* nop */
}

void Scene::Draw() {
	if (child) child->Draw();
	else this->DrawScene();
}

Scene* Scene::Ancest() {
	if (!parent) return this;
	else return parent->Ancest();
}

Scene* Scene::Descendant() {
	if (!child) return this;
	else return child->Descendant();
}


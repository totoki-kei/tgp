#include "Scene.h"


Scene::Scene() : parent{ nullptr }, child{ nullptr } {}
Scene::Scene(Scene* p) : parent{ p }, child{ nullptr } {}

// *** �������z�֐������N���X�̃C���X�^���X�̓��[�u�ł��Ȃ�
//Scene::Scene(Scene&& s) : parent{ s.parent }, child{ s.child } {
//	// ���[�u�R���X�g���N�^�͕K�v���낤���H
//	// �ꉞ��`����B
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
		// �T�u�V�[��������ꍇ�͂�������s
		bool pop = false;
		auto s = child->Update();

		if (s == child) {
			// �߂�l���T�u�V�[�����̂��� -> �������Ȃ�(�����������̃T�u�V�[�����g�p����)
			ret = this;
		}
		else if(s == this) {
			// �߂�l������ -> �T�u�V�[���I��
			// �T�u�V�[�����炱�̃V�[���ւ̒l�̎󂯓n����
			// ����ȑO�ɃT�u�V�[�����ɂčs��ꂽ���̂Ƃ���B
			delete child;
			child = nullptr;
			// �������g��Ԃ��āA���̃V�[�����ێ������I��
			ret = this;
		}
		else if (s == nullptr) {
			// �߂�l��nullptr -> �T�u�V�[���I��
			// this�̃P�[�X�ƈقȂ�A�������g�̃V�[���𑦍��Ɏ��s����
			delete child;
			child = nullptr;
			// �V�[���̃A�b�v�f�[�g�����s�����̌��ʂ�Ԃ�
			ret = this->UpdateScene();
		}
		else {
			// �߂�l�������ȊO�̃V�[�� -> �T�u�V�[�������ւ�
			delete child;
			child = s;
			// �������g��Ԃ��āA���̃V�[�����ێ������I��
			ret = this;
		}

	}
	else {
		// �T�u�V�[���������ꍇ�͎��������s
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


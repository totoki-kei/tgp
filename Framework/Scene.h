#pragma once

class Scene {
protected:
	Scene *parent, *child;

	// �V�[�����X�V����
	// �߂�l�́A�ȉ��̂��������ꂩ�Ƃ���
	// this        : ���݂̃V�[�����p���A�܂��̓T�u�V�[���֑J�ڂ���
	// parent      : ���̃V�[�����I������
	// nullptr     : ���̃V�[�����I�����A�����ɐe�V�[�������s����
	// <new Scene> : ���̃V�[�����I�����A�Ԃ����V�[���ɍ����ւ���
	//
	// <MEMO>
	//   �T�u�V�[���֑J�ڂ���ꍇ�́AUpdateScene����
	//   child�֐V�����V�[����ݒ肷��B
	virtual Scene* UpdateScene();

	virtual void DrawScene();

public:
	Scene();
	Scene(Scene* parent);
	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;

	virtual ~Scene();

	Scene* Update();
	void Draw();

	Scene* Ancest();
	Scene* Descendant();
};


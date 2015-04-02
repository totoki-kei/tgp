#pragma once

#define RSC_DBG

#include <memory>
#include <vector>
#include <set>
#include <functional>

#include "Debug.h"
#include "WinAPI.h"

#ifdef RSC_DBG
#define RSC_DBG_OUT LOG_DBG
#else
#define RSC_DBG_OUT __noop
#endif


class Resource{
	/*
	Resource�̃T�u�N���X�́A�����œƎ��ɍ쐬�������\�[�X(�n���h����|�C���^)��
	AddResource���Ď��g�ɓo�^���Ă������ƂŁA�폜���Ɏ����I�ɍ폜�����B
	�������A���ۂɍ폜���s����̂�Resource�̃f�X�g���N�^���Ă΂ꂽ���Ȃ̂�
	�T�u�N���X���g�̃f�X�g���N�^�����܂łɍ폜�ς݂ł���K�v�����郊�\�[�X��
	�ȉ��̂ǂ��炩�őΉ����邱�ƁB
	 (1) �f�X�g���N�^�� �uif(!isDisposed()) Dispose();�v���s��
	 (2) AddResource�����ɁA�Ǝ��ɊǗ����ăf�X�g���N�^�Łu�ʏ�ʂ�v�J������

	�Ȃ��A�폜���̓��쏇���͈ȉ��̂悤�ɂȂ�F
	1. �T�u�N���X�̃f�X�g���N�^���Ă΂��(�K�v�ɉ����ă��\�[�X�̉��(Dispose�Ăяo���܂�)���s��)
	2. Resource�̃f�X�g���N�^���Ă΂��
	2.1. isDisposed���ĂсADispose���Ă΂ꂽ���m�F����(�I�[�o�[���C�h���Ă���΂��ꂪ�Ă΂��)
	2.2. ����isDisposed��false�ł����AddResource����Ă��郊�\�[�X��S��Dispose����
	3. �f�X�g���N�^���������폜�����

	�y���Ӂz
	���̃N���X�́A�e���\�[�X��������ꂽ�Ƃ��Ɏq���\�[�X�̉���R���h���ړI�̕��ł���
	�g���Ȃ��Ȃ������\�[�X������������邽�߂̕��ł͂Ȃ��B
	�ނ���AddResource���ꂽ�C���X�^���X�́A�e���\�[�X�Ƀn���h������Ă������
	���ۂ�delete�ƃ���������͍s���Ȃ��B
	�蓮�ŊJ���������ꍇ�́A�ȉ��̎菇�Őe���\�[�X�����菜������
	(1) �e���\�[�X.FindResource(�q���\�[�X��ID) ��weak_ptr�擾
	(2) weak_ptr�����b�N����shared_ptr�擾
	(3) �擾����shared_ptr���g�p���Đe���\�[�X��RemoveResource���ĂԁB
	*/

protected:
	typedef std::set< std::shared_ptr<Resource> > children_type;
	// �o�^����Ă��郊�\�[�X
	children_type* children;

	// ���g�̃��\�[�XID
	unsigned int ResourceId;

public:
	// �����폜����郊�\�[�X��o�^����
	// �o�^���ꂽ���\�[�X�̃|�C���^�����̂܂ܕԂ�
	std::shared_ptr<Resource> AddResource(std::shared_ptr<Resource>);

	// �o�^����Ă��郊�\�[�X����폜����
	bool RemoveResource(std::shared_ptr<Resource>, bool all = false, bool recursive = false);
	bool RemoveResource(Resource*, bool all = false, bool recursive = false);

	// ���̃��\�[�X�̃��\�[�XID�𓾂�
	unsigned int GetResourceID() const;

	// �o�^����Ă��郊�\�[�X����w��̃��\�[�XID�����������\�[�X����������
	std::weak_ptr<Resource> FindResource(unsigned int id, bool recursive = false);
	
	// �R���X�g���N�^�A���\�[�XID�������I�Ɋ��蓖�Ă�
	Resource(void);
	// �R���X�g���N�^�A�w��̃��\�[�XID���g�p����
	Resource(unsigned int);

	// ���\�[�X����ς݂̏ꍇ��true��Ԃ��B
	// ����̎����ł́A�o�^����Ă��郊�\�[�X�����݂��Ȃ��A�܂��͂��ׂĉ���ς݂ł���ꍇtrue�ƂȂ�
	// [�I�[�o�[���C�h���̋K��] Dispose() ���Ă΂ꂽ��́A���Ȃ炸true��Ԃ��悤�ɂ��邱��
	virtual bool isDisposed();

	// ���\�[�X���J������
	// ����̎����ł́A�o�^����Ă��郊�\�[�X�����ׂĉ��(Dispose)���A�o�^��������
	// [�I�[�o�[���C�h���̋K��] �K��Resource::Dispose()��1��ȏ�ĂԂ���
	virtual void Dispose();

	// �o�^����Ă��郊�\�[�X�����ׂĉ�����A�o�^��������
	void DisposeResources();

	// �f�X�g���N�^
	// ����̎����ł́A����isDisposed()��false�̎��ɂ�Dispose���Ă�
	virtual ~Resource();

private:

	static unsigned int nextid;

};


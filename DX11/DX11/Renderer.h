#pragma once
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"
#include "Component.h"

class Renderer : public Component
{
public:

	//�g�����X�t�H�[�����
	//Transform* m_pTransform;

	//���b�V�����@
	//�i���̓e�N�X�`���̓��b�V���������Ă���A�{���̓}�e���A������������ׂ��ł���j
	MeshGroup* m_pMeshGroup;

	//�}�e���A�����i�V�F�[�_�[�j
	Material* m_pMaterial;

	Renderer();
	~Renderer();

	HRESULT SetMaterial(Material* pMaterial);

	//�o�b�`���O�̂��߁A���̒i�K�ł̓����_�����O�L���[�ɓo�^�������s��
	//�Q�[���I�u�W�F�N�g���Ƃɂ���Update�̒����烌���_�����O�����s����̂ł͌����������B
	//���̊֐��ł́A�����_�����O�V�X�e���Ƀ��N�G�X�g�𑗂��āA�g�p���郁�b�V���̃C���X�^���X�����蓖�ĂĂ��炤�i�����ǂ����ɂ�����炸�j
	//�����_�����O�V�X�e���́A���b�V���C���X�^���X�̂̏W��������I�Ɉێ��E�Ǘ����Ă��āA�����^�C���ł̏��������������邽�߂Ƀo�b�`�������s��
	//���̊֐��ł�邱�Ƃ́A�����_�����O�V�X�e���Ƀ��N�G�X�g�𑗂邩�A���b�V���C���X�^���X�̃v���p�e�B��ύX���邾��
	//�����_�����O��@�iFoward�����_�����O���ADeffered�����_�����O���ɂ�����āA�����͂�����Ă��邾�낤�j
	HRESULT Render();

	//�V�������b�V���C���X�^���X�̊��蓖�ă��N�G�X�g���o��

	bool IsRenderable();


	
};


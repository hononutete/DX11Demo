#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

class MeshInstance
{
public:
	MeshInstance();
	~MeshInstance();

	//�g�����X�t�H�[�����
	Transform* m_pTransform;

	//���b�V�����@
	//�i���̓e�N�X�`���̓��b�V���������Ă���A�{���̓}�e���A������������ׂ��ł���j
	MeshGroup* m_pMeshGroup;

	//�}�e���A�����i�V�F�[�_�[�j
	Material* m_pMaterial;

	//���̃��b�V���C���X�^���X�̕`������s����
	//���̊֐��̒���DirectX���Ăяo���ă����_�����O�����s���Ă��܂��ƃo�b�`�������ł��Ȃ�
	//�o�b�`�����͈ꏏ�Ƀ����_�����O�ł�����̂���x�̃����_�����O�Ŏ��s������́A�܂葼�̃��b�V���C���X�^���X�̏�񂪕K�v�ɂȂ�
	//�܂背���_�����O�͍����x���ȂƂ���ōs��
	HRESULT Render();

};


#pragma once
//#include "GameObject.h"

//�O���錾
//GameObject�N���X���g����悤�ɂ��邽��
class GameObject;

class Component
{
public:
	//���̃R���|�[�l���g�������Ă���GameObject�ւ̃|�C���^
	GameObject* m_pGameObject;
	
	//�f�t�H���g�R���X�g���N�^
	Component();

	//�R�s�[�R���X�g���N�^
	Component(Component& rother);

	//�f�X�g���N�^
	virtual ~Component();

	//�R���|�[�l���g�̃t���[������
	void Update();
};


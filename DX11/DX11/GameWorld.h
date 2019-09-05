#pragma once
#include "GameObject.h"
#include "FBXSDKMeshLoader.h"
#include "InputOperatorCamera.h"
#include "Level.h"
#include <vector>
//�O���錾
class Camera;

/*
�Q�[�����[���h�̃N���X
���[���h��ɑ��݂��邷�ׂẴQ�[���I�u�W�F�N�g�̃|�C���^�̃��X�g���������Ă���B

�����̃��x�����V�[����ɓǂݍ��ނ��Ƃ��ł���悤�ɂ���K�v������

*/

/*
���x���͈�̃��[���h�`�����N�ɑ�������N���X

���x���͔񓯊��œǂݍ��݂��\�ł���A�����̃��x�����V�[����ɓǂݍ��߂邱�Ƃ��K�v��
������񓯊��Ŕj�����ł��Ȃ���΂����Ȃ��A���̎��ɂǂ̃Q�[���I�u�W�F�N�g��j������΂����̂��m�邽�߂�
�Q�[���I�u�W�F�N�g�ƃ��x���̕R�Â��͕K�v�ł���

���x���̃��[�h�ɂ̓������̊Ǘ������ɂȂ��Ă���
���x���̒��Ŏg���Q�[���I�u�W�F�N�g�����炩���ߌ��܂��Ă���΂��炩���߃��x�������[�h���郁�����̑��ʂ��킩��
���������x�����ɓ��I�ɃQ�[���I�u�W�F�N�g�����[�h���Ȃ���΂����Ȃ��ꍇ������A���̏ꍇ�̓t���O�����e�[�V�����ɋC��t���Ȃ���΂����Ȃ�
���x�����ŁA�g���܂킹��Q�[���I�u�W�F�N�g�͂Ȃ�ׂ��g���܂킷���Ƃœ��I�������̃��[�h��h�����Ƃ��ł���

���x���ł́A���x�����̃Q�[���I�u�W�F�N�g���Ǘ�����



���x���N���X�����ׂ����́A�V���A���C�Y���ꂽ�Q�[���I�u�W�F�N�g�f�[�^�B�����ł����V���A���C�Y���ꂽ�f�[�^�Ƃ�


���x�����̃Q�[���I�u�W�F�N�g�͕K�������A�Z�b�g�Ƃ��đ��݂��Ȃ��Ă�����
���x�����g���Q�[���I�u�W�F�N�g��ێ�����

*/

class GameWorld
{
public:

	//���s�����iDirectonal Light�j�@���[���h��ԂŒ�`
	XMFLOAT4 m_vecLight = XMFLOAT4(-5.0f, -5.0f, 10.0f, 0.0f);

	//�����@(0.0 �` 1.0�Ŏw��j
	float m_Ambient = 0.0f;

	//�Q�[���I�u�W�F�N�g
	GameObject* m_pBox;
	GameObject* m_pPlane;
	GameObject* m_pCamera;

	//���[���h���Ƀ��[�h����Ă��郌�x���̃|�C���^���X�g
	std::vector<GameObject*> m_pLoadedLevelList;

	//���[���h���̂��ׂẴQ�[���I�u�W�F�N�g�̃|�C���^���X�g
	std::vector<GameObject*> m_pGameObjectList;

	//�J�����ւ̃|�C���^�i���̃I�u�W�F�N�g�̓J�����̃Q�[���I�u�W�F�N�g�ŃR���|�[�l���g�����Ƃ��ĊǗ������A���̃N���X�ł̓������Ǘ����s��Ȃ��j
	Camera* m_pCameraComponent;

	//���͂���������N���X
	InputOperatorCamera* m_pInputOperatorCamera;

	//�R���X�g���N�^�A�f�X�g���N�^
	GameWorld();
	~GameWorld();

	//�Q�[�����[���h�Ƀ��x�������[�h����ATODO:�@���x�������[�h����Ƃ��Ƀ��[���h�̂ǂ̍��p����ɁA�ǂ̉�]�ŁA���[�h����̂���offset�̎w�肪�K�v
	HRESULT LoadLevel(UINT levelID);

	//���[�h�ς݃��x�����A�����[�h����
	HRESULT DestroyLevel(UINT levelID);

	//�V�[���̃t���[������
	void UpdateGameWorld();

	//�V�[���̃����_�����O
	HRESULT RenderGameWorld();
};


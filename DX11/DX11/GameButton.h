#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "GameController.h"
using namespace DirectX;


enum GAMEBUTTON_KEY_ID {
	//key id for windows api
	GBWA_KEY_ID_W = 87,
	GBWA_KEY_ID_A = 'A',
	GBWA_KEY_ID_S = 'S',
	GBWA_KEY_ID_D = 'D',
	//GBWA_KEY_ID_W = 'W',
	//GBWA_KEY_ID_A = 'A',
	//GBWA_KEY_ID_S = 'S',
	//GBWA_KEY_ID_D = 'D',

	GBWA_KEY_ID_DIRECT,
	GBWA_KEY_ID_MOUSE1,
	GBWA_KEY_ID_NONE = 0
};

//this is interface for each single button
//GameController�i���z�Q�[���R���g���[���[�j���́A�ЂƂ̃{�^���̃C���^�[�t�F�C�X�ƂȂ�N���X
//��ɁA�{�^���̏�Ԃ̎擾�A�L�[�R���t�B�O�̐ݒ�Ȃǂ��ł���
class GameButton
{
public:
	GAMEBUTTON_KEY_ID m_keyID;
	int mkeyID;

	GameButton();
	~GameButton();


	//value for the button, for simple button this might be 0 or 1. but for joystick, this could be a deciaml value like 0.1 
	//�{�^���̗ʂ��擾�B�ʏ�0��1�����A�W���C�X�e�B�b�N�Ȃǂ̏ꍇ�͓��͉����Ȃǂ����邽��
	virtual XMFLOAT3 GetValue() = 0;

	//set key config, which gamecontroller button to set for the input device button
	//���̃{�^����KeyConfig��ݒ肷��B�܂�A���̉��z�Q�[���{�^�������ۂɎg�p������̓{�^���̂ǂ̃{�^���ɂ�����̂���ݒ肷��
	virtual bool SetConfig(GAMEBUTTON_KEY_ID keyID) = 0;
};


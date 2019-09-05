#include "stdafx.h"
#include "GameMouseButtonWAsync.h"
#include "Debug.h"

GameMouseButtonWAsync::GameMouseButtonWAsync()
{
}


GameMouseButtonWAsync::~GameMouseButtonWAsync()
{
}

//value for the button, for simple button this might be 0 or 1. but for joystick, this could be a deciaml value like 0.1 
//�{�^���̗ʂ��擾�B�ʏ�0��1�����A�W���C�X�e�B�b�N�Ȃǂ̏ꍇ�͓��͉����Ȃǂ����邽��
XMFLOAT3 GameMouseButtonWAsync::GetValue()
{
	extern HWND hWnd;

	POINT p;
	//�f�X�N�g�b�v��ł̃X�N���[�����W���擾
	GetCursorPos(&p);

	//�N���C�A���g�̈�ł̍��W�ɕϊ�
	ScreenToClient(hWnd, &p);

	////�N���C�A���g�̈���W�ɕϊ�
	//Debug::Log(_T("x: %ld, y: %ld\n"), p.x, p.y);

	//return XMFLOAT3(p.x, p.y, 0);
	return XMFLOAT3(p.x, p.y, 0);
}

//set key config, which gamecontroller button to set for the input device button
//���̃{�^����KeyConfig��ݒ肷��B�܂�A���̉��z�Q�[���{�^�������ۂɎg�p������̓{�^���̂ǂ̃{�^���ɂ�����̂���ݒ肷��
//WindowsAPI���g���ꍇ�AWindowsAPI�Œ�`����Ă��鉼�z�L�[�R�[�h���g�p����B���̉��z�L�[�R�[�h��windows.h�̒��i�����ɂ͈Ⴄ�w�b�_�[�t�@�C����������Ȃ����j
//�Œ�`����Ă��āA�ŏI�I�Ȍ^��int�^�ł���
bool GameMouseButtonWAsync::SetConfig(GAMEBUTTON_KEY_ID keyID) {
	m_keyID = keyID;
	//mkeyID = 3;
	////OutputDebugString(_T("set config\n"));
	//m_keyID = GBWA_KEY_ID_W;
	//GAMEBUTTON_KEY_ID m = m_keyID;
	return true;
}

#include "stdafx.h"
#include "GameButtonWAsync.h"
#include <Windows.h>

GameButtonWAsync::GameButtonWAsync()
{
	m_keyID = GBWA_KEY_ID_NONE;
}


GameButtonWAsync::~GameButtonWAsync()
{
}

//value for the button, for simple button this might be 0 or 1. but for joystick, this could be a deciaml value like 0.1 
//�{�^���̗ʂ��擾�B�ʏ�0��1�����A�W���C�X�e�B�b�N�Ȃǂ̏ꍇ�͓��͉����Ȃǂ����邽��
XMFLOAT3 GameButtonWAsync::GetValue() 
{

	//GAMEBUTTON_KEY_ID eKid = m_keyID;
	//TCHAR debugStr[512];
	//_stprintf_s(debugStr, 512, _T(" --- Enum kid RAW [%d]\n"), eKid);
	//OutputDebugString(debugStr);
	
	//�L�[��������Ă���΍ŏ�ʃr�b�g�ɂP���߂�A�ŏ�ʃr�b�g��1��16�i���i0x8000�j��AND�r�b�g���Z�q�ōŏ�ʃr�b�g���m�F����
	if (GetAsyncKeyState(m_keyID) & 0x8000) {
		return XMFLOAT3(1, 1, 1);
	}
	return XMFLOAT3(0, 0, 0);
}

//set key config, which gamecontroller button to set for the input device button
//���̃{�^����KeyConfig��ݒ肷��B�܂�A���̉��z�Q�[���{�^�������ۂɎg�p������̓{�^���̂ǂ̃{�^���ɂ�����̂���ݒ肷��
//WindowsAPI���g���ꍇ�AWindowsAPI�Œ�`����Ă��鉼�z�L�[�R�[�h���g�p����B���̉��z�L�[�R�[�h��windows.h�̒��i�����ɂ͈Ⴄ�w�b�_�[�t�@�C����������Ȃ����j
//�Œ�`����Ă��āA�ŏI�I�Ȍ^��int�^�ł���
bool GameButtonWAsync::SetConfig(GAMEBUTTON_KEY_ID keyID) {
	m_keyID = keyID;
	//mkeyID = 3;
	////OutputDebugString(_T("set config\n"));
	//m_keyID = GBWA_KEY_ID_W;
	//GAMEBUTTON_KEY_ID m = m_keyID;
	return true;
}

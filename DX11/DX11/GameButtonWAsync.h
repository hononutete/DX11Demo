#pragma once
#include "GameButton.h"

class GameButtonWAsync : public GameButton
{
public:
	GameButtonWAsync();
	~GameButtonWAsync();

	//value for the button, for simple button this might be 0 or 1. but for joystick, this could be a deciaml value like 0.1 
	//�{�^���̗ʂ��擾�B�ʏ�0��1�����A�W���C�X�e�B�b�N�Ȃǂ̏ꍇ�͓��͉����Ȃǂ����邽��
	XMFLOAT3 GetValue() override;

	//set key config, which gamecontroller button to set for the input device button
	//���̃{�^����KeyConfig��ݒ肷��B�܂�A���̉��z�Q�[���{�^�������ۂɎg�p������̓{�^���̂ǂ̃{�^���ɂ�����̂���ݒ肷��
	bool SetConfig(GAMEBUTTON_KEY_ID keyID) override;
};


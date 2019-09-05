#include "stdafx.h"
#include "GameController.h"
#include "GameButton.h"
#include "GameButtonWAsync.h"
#include "GameMouseButtonWAsync.h"
#include "Debug.h";
GameController::GameController()
{
}


GameController::~GameController()
{
}

//initialize devices
void GameController::Initialize() {
	//�ǂ̓��̓f�o�C�X���g�p�\���`�F�b�N

	//�g�p������̓f�o�C�X�̑I���A�r���Ő؂�ւ���ꍇ���l������ƑS�Ă�?
	//��������L�[�{�[�h�ɐݒ�
	m_gc_device_type = GC_DEVICT_TYPE_KEYBOARD_AND_MOUSE;

	//���̓f�o�C�X�̏�����

	//�g�p������̓f�o�C�X�ɉ����āA���̃f�o�C�X�̎�ނ̉��z�{�^�����쐬����
	if (m_gc_device_type == GC_DEVICT_TYPE_KEYBOARD_AND_MOUSE)
	{
		InitForWAsync();
	}
	else {
		OutputDebugString(_T("error : no input device for controller"));
	}
}

//update device input state
void GameController::Update() {
	//for windows API, you dont need to update because you can get device info just from an API

	//update mouse position //TODO: do this only when you have mouse


	m_CurrentFrameMousePos = m_btnMap[GCBTN_DIRECT]->GetValue();
	////load to SIMD 
	XMVECTOR vCurrent = XMLoadFloat3(&m_CurrentFrameMousePos);
	XMVECTOR vLast = XMLoadFloat3(&m_LastFrameMousePos);

	//calculate in SIMD
	XMVECTOR vSub = vCurrent - vLast;

	//store to heap memory
	XMStoreFloat3(&m_FrameMousePosDiff, vSub);
	//Debug::Log(_T("current = %f, %f, %f\n"), m_CurrentFrameMousePos.x, m_CurrentFrameMousePos.y, m_CurrentFrameMousePos.z);
	//Debug::Log(_T("last = %f, %f, %f\n"), m_LastFrameMousePos.x, m_LastFrameMousePos.y, m_LastFrameMousePos.z);
	//Debug::Log(_T("last = %f\n"), m_LastFrameMousePos);
	//Debug::Log(_T("mouseMove = %f, %f, %f\n"), m_FrameMousePosDiff.x, m_FrameMousePosDiff.y, m_FrameMousePosDiff.z);
	m_LastFrameMousePos = m_CurrentFrameMousePos;
}


bool GameController::GetButton(GAMECONTROLLER_BTN btnID, GameButton** ppGameButton)
{
	*ppGameButton = m_btnMap[btnID];
	return true;
}

//�}�E�X�̈ʒu���������Ƀs�N�Z�����Ŏ擾
XMFLOAT3 GameController::GetMousePosition() {
	return m_CurrentFrameMousePos;
}

//�}�E�X�̈ʒu��0~1�̊Ԃɐ��K�������l��Ԃ�
XMFLOAT3 GameController::GetMousePositionNormalized() {
	extern DWORD g_Width, g_Height;
	XMFLOAT3 val = m_btnMap[GCBTN_DIRECT]->GetValue();
	return XMFLOAT3(val.x / (float)g_Width, val.y / (float)g_Height, 0.0f);

}

//�O�t���[������̃}�E�X�̈ړ��x�N�g�����擾
XMFLOAT3 GameController::GetMouseMove() {
	return m_FrameMousePosDiff;
}

//initialize controller for input using GetAsyncKeyState funcion from windows api
void GameController::InitForWAsync()
{
	//create btns
	CreateBtnForWAsync();

	//set default config
	SetDefaultConfigForWAsync();
}

void GameController::CreateBtnForWAsync() 
{
	m_btnMap[GCBTN_W] = new GameButtonWAsync();
	m_btnMap[GCBTN_A] = new GameButtonWAsync();
	m_btnMap[GCBTN_S] = new GameButtonWAsync();
	m_btnMap[GCBTN_D] = new GameButtonWAsync();

	m_btnMap[GCBTN_MOUSE1] = new GameMouseButtonWAsync();//mouse button
	m_btnMap[GCBTN_DIRECT] = new GameMouseButtonWAsync();//mouse button
	m_btnMap[GCBTN_NO] = new GameButtonWAsync();
	
}

void GameController::SetDefaultConfigForWAsync() {
	m_btnMap[GCBTN_W]->SetConfig(GBWA_KEY_ID_W);
	m_btnMap[GCBTN_A]->SetConfig(GBWA_KEY_ID_A);
	m_btnMap[GCBTN_S]->SetConfig(GBWA_KEY_ID_S);
	m_btnMap[GCBTN_D]->SetConfig(GBWA_KEY_ID_D);

	m_btnMap[GCBTN_DIRECT]->SetConfig(GBWA_KEY_ID_DIRECT);
	m_btnMap[GCBTN_MOUSE1]->SetConfig(GBWA_KEY_ID_DIRECT);
	//you dont have to set key for direct

	//GameButton* lpGameButton = m_btnMap[GCBTN_W];
	////lpGameButton->m_keyID;
	//int kid = lpGameButton->mkeyID;
	//GAMEBUTTON_KEY_ID eKid = lpGameButton->m_keyID;
	//TCHAR debugStr[512];
	//_stprintf_s(debugStr, 512, _T(" --- Enum kid [%d]\n"), eKid);
	//OutputDebugString(debugStr);
	//_stprintf_s(debugStr, 512, _T(" --- kid [%d]\n"), kid);
	//OutputDebugString(debugStr);
}
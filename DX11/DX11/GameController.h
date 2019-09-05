#pragma once
#include <vector>
#include <unordered_map>
#include <DirectXMath.h>
using namespace DirectX;

//�O���錾
class GameButton;

//buttons of controller is based of PC game, which is keyboard and mouse
enum GAMECONTROLLER_BTN {
	GCBTN_W,
	GCBTN_A,
	GCBTN_S,
	GCBTN_D,

	GCBTN_DIRECT = 0xff00, //mouse button
	GCBTN_NO = 0xffff, //no sign 
	GCBTN_MOUSE1 //mouse button position
};

enum GAMECONTROLLER_DEVICE_TYPE {
	GC_DEVICT_TYPE_KEYBOARD_AND_MOUSE
};
//enum GAMECONTROLLER_BTN {
//	GCBTN_WASYNC_W = 'W',
//	GCBTN_WASYNC_A = 'A',
//	GCBTN_WASYNC_S = 'S',
//	GCBTN_WASYNC_D = 'D',
//
//	GCBTN_WASYNC_DIRECT = 0xff00, //mouse button
//	GCBTN_WASYNC_NO = 0xffff, //no sign 
//};

//virtual controller used for game playing.
//by giving the buttonid (buttonID describes which button in controller), you can get button state for the specified buttonID
//button state can inform you if its pressed of not, and so on

//���̃N���X�͉��z�R���g���[���[�i�Q�[���Ŏg�p����@�\����������R���g���[���[�j�̃N���X�ł���
//��ɁA���z�R���g���[���[�̂ǂ̃{�^�����H��\���{�^��ID�iGAMECONTROLLER_BTN�j��n�����ƂŁA���̃{�^���̏�Ԃ��擾�ł���
//�擾�����{�^���̏�Ԃ́A�{�^����������Ă��邩���Ȃ����A�{�^���̃R���t�B�O�Ȃǂ����߂�΂����A���̋�̓I�Ȏ�����GameButton�̂ق��ɂ��邩�炻�����Q��

class GameController
{
public:
	GameController();
	~GameController();

	//a map of buttons for controller
	//���z�R���g���[���[�̃{�^���ւ̃|�C���^
	std::unordered_map<GAMECONTROLLER_BTN, GameButton*> m_btnMap;

	GAMECONTROLLER_DEVICE_TYPE m_gc_device_type;

	//initialize devices
	void Initialize();

	//update device input state
	void Update();

	//get if button is pressed down or not
	//���z�R���g���[���[��̃{�^�����A�L�[���w�肵�Ď擾
	bool GetButton(GAMECONTROLLER_BTN btnID, GameButton** ppGameButton);

	//�}�E�X�̈ʒu���擾
	XMFLOAT3 GetMousePosition();

	//�}�E�X�̈ʒu���������Ƀs�N�Z�����Ŏ擾
	XMFLOAT3 GetMousePositionNormalized();

	//�O�t���[������̃}�E�X�̈ړ��x�N�g�����擾
	XMFLOAT3 GetMouseMove();

private:
	
	XMFLOAT3 m_CurrentFrameMousePos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_LastFrameMousePos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 m_FrameMousePosDiff = XMFLOAT3(0.0f, 0.0f, 0.0f);

	//initialize controller for input using GetAsyncKeyState funcion from windows api
	void InitForWAsync();

	void CreateBtnForWAsync();

	void SetDefaultConfigForWAsync();
};


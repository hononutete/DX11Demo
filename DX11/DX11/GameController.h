#pragma once
#include <vector>
#include <unordered_map>
#include <DirectXMath.h>
using namespace DirectX;

//前方宣言
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

//このクラスは仮想コントローラー（ゲームで使用する機能が備わったコントローラー）のクラスである
//主に、仮想コントローラーのどのボタンか？を表すボタンID（GAMECONTROLLER_BTN）を渡すことで、そのボタンの状態を取得できる
//取得したボタンの状態は、ボタンが押されているかいないか、ボタンのコンフィグなどをきめればいい、その具体的な実装はGameButtonのほうにあるからそこを参照

class GameController
{
public:
	GameController();
	~GameController();

	//a map of buttons for controller
	//仮想コントローラーのボタンへのポインタ
	std::unordered_map<GAMECONTROLLER_BTN, GameButton*> m_btnMap;

	GAMECONTROLLER_DEVICE_TYPE m_gc_device_type;

	//initialize devices
	void Initialize();

	//update device input state
	void Update();

	//get if button is pressed down or not
	//仮想コントローラー上のボタンを、キーを指定して取得
	bool GetButton(GAMECONTROLLER_BTN btnID, GameButton** ppGameButton);

	//マウスの位置を取得
	XMFLOAT3 GetMousePosition();

	//マウスの位置を左上を基準にピクセル数で取得
	XMFLOAT3 GetMousePositionNormalized();

	//前フレームからのマウスの移動ベクトルを取得
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


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
//GameController（仮想ゲームコントローラー）内の、ひとつのボタンのインターフェイスとなるクラス
//主に、ボタンの状態の取得、キーコンフィグの設定などができる
class GameButton
{
public:
	GAMEBUTTON_KEY_ID m_keyID;
	int mkeyID;

	GameButton();
	~GameButton();


	//value for the button, for simple button this might be 0 or 1. but for joystick, this could be a deciaml value like 0.1 
	//ボタンの量を取得。通常0か1だが、ジョイスティックなどの場合は入力加減などがあるため
	virtual XMFLOAT3 GetValue() = 0;

	//set key config, which gamecontroller button to set for the input device button
	//このボタンのKeyConfigを設定する。つまり、この仮想ゲームボタンが実際に使用する入力ボタンのどのボタンにあたるのかを設定する
	virtual bool SetConfig(GAMEBUTTON_KEY_ID keyID) = 0;
};


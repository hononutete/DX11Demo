#pragma once
#include "GameButton.h"

class GameButtonWAsync : public GameButton
{
public:
	GameButtonWAsync();
	~GameButtonWAsync();

	//value for the button, for simple button this might be 0 or 1. but for joystick, this could be a deciaml value like 0.1 
	//ボタンの量を取得。通常0か1だが、ジョイスティックなどの場合は入力加減などがあるため
	XMFLOAT3 GetValue() override;

	//set key config, which gamecontroller button to set for the input device button
	//このボタンのKeyConfigを設定する。つまり、この仮想ゲームボタンが実際に使用する入力ボタンのどのボタンにあたるのかを設定する
	bool SetConfig(GAMEBUTTON_KEY_ID keyID) override;
};


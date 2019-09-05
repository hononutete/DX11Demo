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
//ボタンの量を取得。通常0か1だが、ジョイスティックなどの場合は入力加減などがあるため
XMFLOAT3 GameButtonWAsync::GetValue() 
{

	//GAMEBUTTON_KEY_ID eKid = m_keyID;
	//TCHAR debugStr[512];
	//_stprintf_s(debugStr, 512, _T(" --- Enum kid RAW [%d]\n"), eKid);
	//OutputDebugString(debugStr);
	
	//キーが押されていれば最上位ビットに１が戻る、最上位ビットが1の16進数（0x8000）とANDビット演算子で最上位ビットを確認する
	if (GetAsyncKeyState(m_keyID) & 0x8000) {
		return XMFLOAT3(1, 1, 1);
	}
	return XMFLOAT3(0, 0, 0);
}

//set key config, which gamecontroller button to set for the input device button
//このボタンのKeyConfigを設定する。つまり、この仮想ゲームボタンが実際に使用する入力ボタンのどのボタンにあたるのかを設定する
//WindowsAPIを使う場合、WindowsAPIで定義されている仮想キーコードを使用する。この仮想キーコードはwindows.hの中（厳密には違うヘッダーファイルかもしれないが）
//で定義されていて、最終的な型はint型である
bool GameButtonWAsync::SetConfig(GAMEBUTTON_KEY_ID keyID) {
	m_keyID = keyID;
	//mkeyID = 3;
	////OutputDebugString(_T("set config\n"));
	//m_keyID = GBWA_KEY_ID_W;
	//GAMEBUTTON_KEY_ID m = m_keyID;
	return true;
}

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
//ボタンの量を取得。通常0か1だが、ジョイスティックなどの場合は入力加減などがあるため
XMFLOAT3 GameMouseButtonWAsync::GetValue()
{
	extern HWND hWnd;

	POINT p;
	//デスクトップ上でのスクリーン座標を取得
	GetCursorPos(&p);

	//クライアント領域での座標に変換
	ScreenToClient(hWnd, &p);

	////クライアント領域座標に変換
	//Debug::Log(_T("x: %ld, y: %ld\n"), p.x, p.y);

	//return XMFLOAT3(p.x, p.y, 0);
	return XMFLOAT3(p.x, p.y, 0);
}

//set key config, which gamecontroller button to set for the input device button
//このボタンのKeyConfigを設定する。つまり、この仮想ゲームボタンが実際に使用する入力ボタンのどのボタンにあたるのかを設定する
//WindowsAPIを使う場合、WindowsAPIで定義されている仮想キーコードを使用する。この仮想キーコードはwindows.hの中（厳密には違うヘッダーファイルかもしれないが）
//で定義されていて、最終的な型はint型である
bool GameMouseButtonWAsync::SetConfig(GAMEBUTTON_KEY_ID keyID) {
	m_keyID = keyID;
	//mkeyID = 3;
	////OutputDebugString(_T("set config\n"));
	//m_keyID = GBWA_KEY_ID_W;
	//GAMEBUTTON_KEY_ID m = m_keyID;
	return true;
}

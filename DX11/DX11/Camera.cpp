#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

//射影変換行列を取得
XMMATRIX Camera::LoadProjectionMatrix()
{
	extern DWORD g_Width, g_Height;
	float aspect = (float)g_Width / (float)g_Height; 
	return XMMatrixPerspectiveFovLH(fieldOfView, aspect, ZNear, ZFar);
}

//ビュー変換行列を取得
XMMATRIX Camera::LoadViewMatrix()
{
	/*TCHAR debugStr[512];
	_stprintf_s(debugStr, 512, _T("matrix[%f, %f, %f]\n"), m_pTransform->transX, m_pTransform->transY, m_pTransform->transZ);
	OutputDebugString(debugStr);*/
	//ワールド座標を使う、ローカル座標ではない 
	//transformのデータを使ってこれらのデータを作る
	XMFLOAT3 eye = XMFLOAT3( m_pTransform->localPosition.x, m_pTransform->localPosition.y, m_pTransform->localPosition.z);
	XMFLOAT3 focus = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//XMFLOAT3 eye = XMFLOAT3(20.0f, 20.0f, -20.0f);
	//XMFLOAT3 focus = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	return XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&focus), XMLoadFloat3(&up));
}

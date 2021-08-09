#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

//�ˉe�ϊ��s����擾
XMMATRIX Camera::LoadProjectionMatrix()
{
	extern DWORD g_Width, g_Height;
	float aspect = (float)g_Width / (float)g_Height; 
	return XMMatrixPerspectiveFovLH(fieldOfView, aspect, ZNear, ZFar);
}

//�r���[�ϊ��s����擾
XMMATRIX Camera::LoadViewMatrix()
{
	/*TCHAR debugStr[512];
	_stprintf_s(debugStr, 512, _T("matrix[%f, %f, %f]\n"), m_pTransform->transX, m_pTransform->transY, m_pTransform->transZ);
	OutputDebugString(debugStr);*/
	//���[���h���W���g���A���[�J�����W�ł͂Ȃ� 
	//transform�̃f�[�^���g���Ă����̃f�[�^�����
	XMFLOAT3 eye = XMFLOAT3( m_pTransform->localPosition.x, m_pTransform->localPosition.y, m_pTransform->localPosition.z);
	XMFLOAT3 focus = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//XMFLOAT3 eye = XMFLOAT3(20.0f, 20.0f, -20.0f);
	//XMFLOAT3 focus = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	return XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&focus), XMLoadFloat3(&up));
}

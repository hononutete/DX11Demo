#include "stdafx.h"
#include "Transform.h"


Transform::Transform()
{
}


Transform::~Transform()
{
}

void Transform::Forward(XMFLOAT3 forward) {
	//���[�J���ł̑O���x�N�g����y���w��x�N�g���ɂȂ�悤�ɉ�]��ݒ�
}

XMFLOAT3 Transform::Forward() {
	return XMFLOAT3(0,0,0);
}

//���̃g�����X�t�H�[����MVP�s���Ԃ��ireturns MVP matrix of this transform�j
XMMATRIX Transform::GetMVPMatrix() {
	//�e�g�����X�t�H�[��������΁A����MVP��������
	if (m_pParent) {

	}
	return XMMatrixIdentity();
}
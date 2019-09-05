#include "stdafx.h"
#include "Transform.h"


Transform::Transform()
{
}


Transform::~Transform()
{
}

void Transform::Forward(XMFLOAT3 forward) {
	//ローカルでの前方ベクトルのyが指定ベクトルになるように回転を設定
}

XMFLOAT3 Transform::Forward() {
	return XMFLOAT3(0,0,0);
}

//このトランスフォームのMVP行列を返す（returns MVP matrix of this transform）
XMMATRIX Transform::GetMVPMatrix() {
	//親トランスフォームがあれば、そのMVPをかける
	if (m_pParent) {

	}
	return XMMatrixIdentity();
}
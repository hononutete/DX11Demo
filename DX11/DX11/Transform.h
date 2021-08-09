#pragma once
#include "DirectXMath.h"
using namespace DirectX;

class Transform
{
public:
	Transform* m_pParent;

	//��]
	//float rotX = 0.0f;
	//float rotY = 0.0f;
	//float rotZ = 0.0f;

	////�ʒu
	//float transX = 0.0f;
	//float transY = 0.0f;
	//float transZ = 0.0f;

	////�X�P�[��
	//float scaleX = 5.0f;
	//float scaleY = 5.0f;
	//float scaleZ = 5.0f;

	XMFLOAT4 eulerAngles   = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	XMFLOAT4 localPosition = XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	XMFLOAT4 scale         = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

	//���[�J����Ԃł̑O���x�N�g�����w��(forward is the y direction of the transform)
	void Forward(XMFLOAT3 forward);
	XMFLOAT3 Forward();

	Transform();
	~Transform();

	XMMATRIX GetMVPMatrix();
};


#pragma once
#include "Component.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "Transform.h"
using namespace DirectX;

class Camera : public Component
{
public:
	Transform* m_pTransform;

	float fieldOfView = 1.5707963f;
	//float aspect = 1.0f;
	float ZNear = 1.0f;
	float ZFar = 100.0f;

	//�ˉe�ϊ��s����擾
	XMMATRIX LoadProjectionMatrix();

	//�r���[�ϊ��s����擾
	XMMATRIX LoadViewMatrix();

	Camera();
	~Camera();
};


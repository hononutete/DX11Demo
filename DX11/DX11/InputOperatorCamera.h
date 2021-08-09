#pragma once
#include "InputOperator.h"
//#include "GameObject.h"
class GameObject;

class InputOperatorCamera : public InputOperator
{
public:

	//target camera
	GameObject* m_pCamera;

	//move speed
	float m_Speed = 10.0f;

	float m_delta = 10.0f;

	InputOperatorCamera();
	~InputOperatorCamera();

	void SetCamera(GameObject* pCameraGameObject);

	void Dispatch() override;
};


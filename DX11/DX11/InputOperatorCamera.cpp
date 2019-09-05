#include "stdafx.h"
#include "InputOperatorCamera.h"
#include "GameController.h"
#include "GameButton.h"
#include "Time.h"
#include "Debug.h"
#include "FrameTes.h" //debug

InputOperatorCamera::InputOperatorCamera()
{
	m_Speed = 3.0f;
}


InputOperatorCamera::~InputOperatorCamera()
{
}

void InputOperatorCamera::Dispatch()
{
	//get controller input
	extern GameController* g_pGameController;
	extern Time* g_pTime;
	extern FrameTes* g_pFrameTes;
	//extern float g_deltaTime;

	GameButton* gameButton;
	//w
	g_pGameController->GetButton(GCBTN_W, &gameButton);
	if (gameButton->GetValue().x == 1) {
		const int a = 1;
		float val = m_pCamera->m_pTransform->transZ;
		float dt = g_pTime->m_deltaTime;
		Debug::Log(_T("z before %f\n"), val);
		Debug::Log(_T("m_deltaTime %f\n"), dt);
		//m_pCamera->m_pTransform->transZ += 3.0f * g_pTime->m_deltaTime;
		m_pCamera->m_pTransform->transZ += 3.0f * 0.01f;
		val = m_pCamera->m_pTransform->transZ;
		Debug::Log(_T("z after %f\n"), val);
	}


	//a
	g_pGameController->GetButton(GCBTN_A, &gameButton);
	if (gameButton->GetValue().x == 1) {

		//g_pFrameTes->time += 1.0f * 
		m_pCamera->m_pTransform->transX -= 3.0f * g_pTime->m_deltaTime;
	}

	//s
	g_pGameController->GetButton(GCBTN_S, &gameButton);
	if (gameButton->GetValue().x == 1) {
		m_pCamera->m_pTransform->transZ -= 3.0f * g_pTime->m_deltaTime;
	}

	//d
	g_pGameController->GetButton(GCBTN_D, &gameButton);
	if (gameButton->GetValue().x == 1) {
		m_pCamera->m_pTransform->transX += 3.0f * g_pTime->m_deltaTime;
	}

	//set camera forward direction
	XMFLOAT3 mouseMove = g_pGameController->GetMouseMove();
	Debug::Log(_T("mouseMove = %f, %f, %f\n"), mouseMove.x, mouseMove.y, mouseMove.z);
	XMFLOAT4 v = m_pCamera->m_pTransform->eulerAngles;
	m_pCamera->m_pTransform->eulerAngles = XMFLOAT4(v.x + mouseMove.x, v.y + mouseMove.y, v.z + mouseMove.z, v.w);

}
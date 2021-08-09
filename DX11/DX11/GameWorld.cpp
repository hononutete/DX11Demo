#include "stdafx.h"
#include "GameWorld.h"
#include "PlaneMeshFactory.h"
#include "FBXSDKMeshLoader.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderCompiler.h"
#include "Renderer.h"
#include "Level.h"
#include "GameObject.h"

GameWorld::GameWorld()
{
	//m_pBox = nullptr;
	//m_pPlane = nullptr;
	m_pCamera = nullptr;
	m_pInputOperatorCamera = nullptr;
}


GameWorld::~GameWorld()
{
	//SAFE_DELETE(m_pBox);
	//SAFE_DELETE(m_pPlane);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pInputOperatorCamera);
}

HRESULT GameWorld::LoadLevel(UINT levelID)
{
	HRESULT hr = E_FAIL;
	extern D3D11User* g_pD3D11User;
	extern FBXSDKMeshLoader* g_pMeshLoader;

	//�V���A���C�Y���ꂽ���x���f�[�^����Q�[�����[���h�ɃI�u�W�F�N�g��z�u
	//TODO:���݂͊ȈՓI�Ƀn�[�h�R�[�h���ꂽ�N���X�����[�h����
	Level* pLevel = new Level();
	pLevel->LoadLevelSync(this);

	//���x���̃��X�g�Ɋi�[
	m_pLoadedLevelList.push_back(pLevel);
	
	//�J�����̍쐬�iGameObject���x�[�X�ɂ���̂́A�Q�[���I�u�W�F�N�g������Transform�f�[�^���g����������B�J�����̈ʒu�A�����Ȃǁj
	m_pCamera = new GameObject();
	m_pCamera->Initialize();
	{
		//...���b�V���Ȃǂ��쐬����K�v�͂Ȃ�

		//�J�����̏����g�����X�t�H�[����ݒ�
		m_pCamera->m_pTransform->localPosition = XMFLOAT4(5.f, 0.f, -20.f, 1.f);
		//m_pCamera->m_pTransform->transX = 5.0f;
		//m_pCamera->m_pTransform->transY = 0.0f;
		//m_pCamera->m_pTransform->transZ = -20.0f;

		//�J�����̃R���|�[�l���g�̍쐬
		m_pCameraComponent = new Camera();            //�V�[���̎������o�[�ϐ��ɏ�����
		m_pCamera->SetComponent(m_pCameraComponent);  //�R���|�[�l���g�̃������Ǘ��̓Q�[���I�u�W�F�N�g�Ɉ�C
		m_pCameraComponent->m_pTransform = m_pCamera->m_pTransform; //�g�����X�t�H�[����ݒ�

	}

	//���͏������s���I�y���[�^�[�N���X�̏�����
	m_pInputOperatorCamera = new InputOperatorCamera();
	m_pInputOperatorCamera->m_pCamera = m_pCamera;

	OutputDebugString(_T("Scene load succeeded!  \n"));
	hr = S_OK;
EXIT:

	return hr;
}

HRESULT GameWorld::DestroyLevel(UINT levelID) {
	return S_OK;
}

//�V�[���̃t���[������
void GameWorld::UpdateGameWorld()
{
	//���ׂẴQ�[���I�u�W�F�N�g�ɑ΂��āA�t���[���������s��
	m_pCamera->Update();
	for (GameObject* pGameObject : m_pGameObjectList)
	{
		pGameObject->Update();
	}

	//���͏������s��
	m_pInputOperatorCamera->Dispatch();
}

HRESULT GameWorld::RenderGameWorld()
{
	HRESULT hr = E_FAIL;
	extern D3D11User* g_pD3D11User;

	//�N���A����
	{
		//�o�b�N�o�b�t�@���N���A
		float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		g_pD3D11User->m_D3DDeviceContext->ClearRenderTargetView(g_pD3D11User->m_RenderTargetView, clearColor);

		//�[�x�o�b�t�@���N���A
		if (g_pD3D11User->m_DepthStencilView)
			g_pD3D11User->m_D3DDeviceContext->ClearDepthStencilView(g_pD3D11User->m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	
	//�Q�[���I�u�W�F�N�g���o�b�N�o�b�t�@�ɕ`��
	{
		//�{���ł���΃����_�����O�̏��Ԃ��l�����Ȃ���΂Ȃ�Ȃ������͌Œ�
		//�S�ẴQ�[���I�u�W�F�N�g�̃����_�����O
		for (GameObject* pGameObject : m_pGameObjectList) 
		{
			pGameObject->Render();
		}
	}

	//�X���b�v�`�F�[���ɂ���ăo�b�N�o�b�t�@�̓��e���t�����g�o�b�t�@�ɔ��f�i�f�B�X�v���C�ɕ\���j
	hr = g_pD3D11User->m_SwapChain->Present(0, 0);
	hr = S_OK;

	return hr;
}

HRESULT GameWorld::AddGameObjectToWolrd(GameObject* pGameObject)
{
	m_pGameObjectList.push_back(pGameObject);
	return S_OK;
}

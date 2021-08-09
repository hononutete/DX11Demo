#include "stdafx.h"
#include "Level.h"
#include "GameWorld.h"
#include "PlaneMeshFactory.h"
#include "FBXSDKMeshLoader.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderCompiler.h"
#include "Renderer.h"
Level::Level()
{
}


Level::~Level()
{
}

HRESULT Level::LoadLevelSync(GameWorld* pWorld) {

	MyOutputDebugString(_T("!!!!!!!! LoadLevelSync start...\n"));
	HRESULT hr = E_FAIL;
	extern D3D11User* g_pD3D11User;
	extern FBXSDKMeshLoader* g_pMeshLoader;

	//�Q�[���I�u�W�F�N�g�̍쐬

	//�{�b�N�X
	m_pBox = new GameObject();
	m_pBox->Initialize();
	{
		m_pBox->m_pTransform->localPosition = XMFLOAT4(0.f, 0.f, 0.f, 1.f);

		//�}�e���A�����쐬
		Material* pMaterial = new Material(_T("DefaultDiffse.hsls"));

		//�����_���[���쐬
		Renderer* pRenderer = new Renderer();

		MyOutputDebugString(_T("Renderer type 1 : %s\n"), typeid(*pRenderer).name());
		MyOutputDebugString(_T("Renderer type 2 : %s\n"), typeid(Renderer).name());


		// �����_���[�R���|�[�l���g�̃��b�V���O���[�v�ɁA���b�V�������[�h���Đݒ�
		hr = g_pMeshLoader->LoadMeshData(_T("C:/Users/yuya/Desktop/GameProjects/DirectX11VS15/DX11/DX11/Resources/animatedBox.fbx"), &pRenderer->m_pMeshGroup);
		//hr = g_pMeshLoader->LoadMeshData(_T("C:/Users/yuya/Desktop/GameProjects/DirectX11VS15/DX11/DX11/Resources/animatedBox.fbx"), &m_pBox->m_pMeshGroup);
		if (FAILED(hr)) {
			OutputDebugString(_T("ERROR : loading fbx failed...\n"));
			goto EXIT;
		}
		//�����_���[�Ƀ}�e���A����ݒ�
		pRenderer->SetMaterial(pMaterial);

		//�����_���[�̃��b�V���Ɉ�ԍŏ��̃A�j���[�V�������Z�b�g
		pRenderer->m_pMeshGroup->Meshes[0].SetAnimation(0);

		//�����_���[���Q�[���I�u�W�F�N�g�ɃZ�b�g
		m_pBox->SetComponent(pRenderer);



		//�f�t�H���g�̃}�e���A�����쐬
		//m_pBox->CreateMaterial(*(_T("DefaultDiffse.hsls")));

		//��ԍŏ��̃A�j���[�V�������Z�b�g
		//m_pBox->m_pMeshGroup->Meshes[0].SetAnimation(0);
	}

	//���[�h�����I�u�W�F�N�g�����[���h�ɒǉ�
	pWorld->AddGameObjectToWolrd(m_pBox);

	//���ʃ|���S��
	//m_pPlane = new GameObject();
	//m_pPlane->Initialize();
	//{
	//	//���b�V���쐬
	//	PlaneMeshFactory planeMeshFactory;
	//	PlaneMeshFactory* lpPlaneMeshFactory = &planeMeshFactory;
	//	hr = lpPlaneMeshFactory->CreateMesh(&m_pPlane->m_pMeshGroup);
	//	if (FAILED(hr)) {
	//		OutputDebugString(_T("ERROR : loading plane mesh failed...\n"));
	//		goto EXIT;
	//	}

	//	//�X�P�[����ݒ�@TODO:�{���̓t�@�N�g���[���ł��
	//	m_pPlane->m_pTransform->scaleX = 20.0f;
	//	m_pPlane->m_pTransform->scaleY = 20.0f;
	//	m_pPlane->m_pTransform->scaleZ = 20.0f;

	//	//�f�t�H���g�̃}�e���A�����쐬
	//	m_pPlane->CreateMaterial(*(_T("DefaultDiffse")));
	//}

	//�J�����̍쐬�iGameObject���x�[�X�ɂ���̂́A�Q�[���I�u�W�F�N�g������Transform�f�[�^���g����������B�J�����̈ʒu�A�����Ȃǁj
	//m_pCamera = new GameObject();
	//m_pCamera->Initialize();
	//{
	//	//...���b�V���Ȃǂ��쐬����K�v�͂Ȃ�

	//	//�J�����̏����g�����X�t�H�[����ݒ�
	//	m_pCamera->m_pTransform->localPosition = XMFLOAT4(5.f, 0.f, -20.f, 1.f);
	//	//m_pCamera->m_pTransform->transX = 5.0f;
	//	//m_pCamera->m_pTransform->transY = 0.0f;
	//	//m_pCamera->m_pTransform->transZ = -20.0f;

	//	//�J�����̃R���|�[�l���g�̍쐬
	//	m_pCameraComponent = new Camera();            //�V�[���̎������o�[�ϐ��ɏ�����
	//	m_pCamera->SetComponent(m_pCameraComponent);  //�R���|�[�l���g�̃������Ǘ��̓Q�[���I�u�W�F�N�g�Ɉ�C
	//	m_pCameraComponent->m_pTransform = m_pCamera->m_pTransform; //�g�����X�t�H�[����ݒ�

	//}

	////���͏������s���I�y���[�^�[�N���X�̏�����
	//m_pInputOperatorCamera = new InputOperatorCamera();
	//m_pInputOperatorCamera->m_pCamera = m_pCamera;

	OutputDebugString(_T("Level load succeeded!  \n"));
	hr = S_OK;
EXIT:

	return hr;
}

HRESULT Level::LoadLevelAsync() {
	return S_OK;
}

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

	//ゲームオブジェクトの作成

	//ボックス
	m_pBox = new GameObject();
	m_pBox->Initialize();
	{
		m_pBox->m_pTransform->localPosition = XMFLOAT4(0.f, 0.f, 0.f, 1.f);

		//マテリアルを作成
		Material* pMaterial = new Material(_T("DefaultDiffse.hsls"));

		//レンダラーを作成
		Renderer* pRenderer = new Renderer();

		MyOutputDebugString(_T("Renderer type 1 : %s\n"), typeid(*pRenderer).name());
		MyOutputDebugString(_T("Renderer type 2 : %s\n"), typeid(Renderer).name());


		// レンダラーコンポーネントのメッシュグループに、メッシュをロードして設定
		hr = g_pMeshLoader->LoadMeshData(_T("C:/Users/yuya/Desktop/GameProjects/DirectX11VS15/DX11/DX11/Resources/animatedBox.fbx"), &pRenderer->m_pMeshGroup);
		//hr = g_pMeshLoader->LoadMeshData(_T("C:/Users/yuya/Desktop/GameProjects/DirectX11VS15/DX11/DX11/Resources/animatedBox.fbx"), &m_pBox->m_pMeshGroup);
		if (FAILED(hr)) {
			OutputDebugString(_T("ERROR : loading fbx failed...\n"));
			goto EXIT;
		}
		//レンダラーにマテリアルを設定
		pRenderer->SetMaterial(pMaterial);

		//レンダラーのメッシュに一番最初のアニメーションをセット
		pRenderer->m_pMeshGroup->Meshes[0].SetAnimation(0);

		//レンダラーをゲームオブジェクトにセット
		m_pBox->SetComponent(pRenderer);



		//デフォルトのマテリアルを作成
		//m_pBox->CreateMaterial(*(_T("DefaultDiffse.hsls")));

		//一番最初のアニメーションをセット
		//m_pBox->m_pMeshGroup->Meshes[0].SetAnimation(0);
	}

	//ロードしたオブジェクトをワールドに追加
	pWorld->AddGameObjectToWolrd(m_pBox);

	//平面ポリゴン
	//m_pPlane = new GameObject();
	//m_pPlane->Initialize();
	//{
	//	//メッシュ作成
	//	PlaneMeshFactory planeMeshFactory;
	//	PlaneMeshFactory* lpPlaneMeshFactory = &planeMeshFactory;
	//	hr = lpPlaneMeshFactory->CreateMesh(&m_pPlane->m_pMeshGroup);
	//	if (FAILED(hr)) {
	//		OutputDebugString(_T("ERROR : loading plane mesh failed...\n"));
	//		goto EXIT;
	//	}

	//	//スケールを設定　TODO:本来はファクトリー内でやる
	//	m_pPlane->m_pTransform->scaleX = 20.0f;
	//	m_pPlane->m_pTransform->scaleY = 20.0f;
	//	m_pPlane->m_pTransform->scaleZ = 20.0f;

	//	//デフォルトのマテリアルを作成
	//	m_pPlane->CreateMaterial(*(_T("DefaultDiffse")));
	//}

	//カメラの作成（GameObjectをベースにするのは、ゲームオブジェクトがもつTransformデータを使いたいから。カメラの位置、向きなど）
	//m_pCamera = new GameObject();
	//m_pCamera->Initialize();
	//{
	//	//...メッシュなどを作成する必要はない

	//	//カメラの初期トランスフォームを設定
	//	m_pCamera->m_pTransform->localPosition = XMFLOAT4(5.f, 0.f, -20.f, 1.f);
	//	//m_pCamera->m_pTransform->transX = 5.0f;
	//	//m_pCamera->m_pTransform->transY = 0.0f;
	//	//m_pCamera->m_pTransform->transZ = -20.0f;

	//	//カメラのコンポーネントの作成
	//	m_pCameraComponent = new Camera();            //シーンの持つメンバー変数に初期化
	//	m_pCamera->SetComponent(m_pCameraComponent);  //コンポーネントのメモリ管理はゲームオブジェクトに一任
	//	m_pCameraComponent->m_pTransform = m_pCamera->m_pTransform; //トランスフォームを設定

	//}

	////入力処理を行うオペレータークラスの初期化
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

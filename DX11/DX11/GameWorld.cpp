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

	//シリアライズされたレベルデータからゲームワールドにオブジェクトを配置
	//TODO:現在は簡易的にハードコードされたクラスをロードする
	Level* pLevel = new Level();
	pLevel->LoadLevelSync(this);

	//レベルのリストに格納
	m_pLoadedLevelList.push_back(pLevel);
	
	//カメラの作成（GameObjectをベースにするのは、ゲームオブジェクトがもつTransformデータを使いたいから。カメラの位置、向きなど）
	m_pCamera = new GameObject();
	m_pCamera->Initialize();
	{
		//...メッシュなどを作成する必要はない

		//カメラの初期トランスフォームを設定
		m_pCamera->m_pTransform->localPosition = XMFLOAT4(5.f, 0.f, -20.f, 1.f);
		//m_pCamera->m_pTransform->transX = 5.0f;
		//m_pCamera->m_pTransform->transY = 0.0f;
		//m_pCamera->m_pTransform->transZ = -20.0f;

		//カメラのコンポーネントの作成
		m_pCameraComponent = new Camera();            //シーンの持つメンバー変数に初期化
		m_pCamera->SetComponent(m_pCameraComponent);  //コンポーネントのメモリ管理はゲームオブジェクトに一任
		m_pCameraComponent->m_pTransform = m_pCamera->m_pTransform; //トランスフォームを設定

	}

	//入力処理を行うオペレータークラスの初期化
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

//シーンのフレーム処理
void GameWorld::UpdateGameWorld()
{
	//すべてのゲームオブジェクトに対して、フレーム処理を行う
	m_pCamera->Update();
	for (GameObject* pGameObject : m_pGameObjectList)
	{
		pGameObject->Update();
	}

	//入力処理を行う
	m_pInputOperatorCamera->Dispatch();
}

HRESULT GameWorld::RenderGameWorld()
{
	HRESULT hr = E_FAIL;
	extern D3D11User* g_pD3D11User;

	//クリア処理
	{
		//バックバッファをクリア
		float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		g_pD3D11User->m_D3DDeviceContext->ClearRenderTargetView(g_pD3D11User->m_RenderTargetView, clearColor);

		//深度バッファをクリア
		if (g_pD3D11User->m_DepthStencilView)
			g_pD3D11User->m_D3DDeviceContext->ClearDepthStencilView(g_pD3D11User->m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	
	//ゲームオブジェクトをバックバッファに描画
	{
		//本来であればレンダリングの順番を考慮しなければならないが今は固定
		//全てのゲームオブジェクトのレンダリング
		for (GameObject* pGameObject : m_pGameObjectList) 
		{
			pGameObject->Render();
		}
	}

	//スワップチェーンによってバックバッファの内容をフロントバッファに反映（ディスプレイに表示）
	hr = g_pD3D11User->m_SwapChain->Present(0, 0);
	hr = S_OK;

	return hr;
}

HRESULT GameWorld::AddGameObjectToWolrd(GameObject* pGameObject)
{
	m_pGameObjectList.push_back(pGameObject);
	return S_OK;
}

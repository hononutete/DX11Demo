// DX11.cpp : Defines the entry point for the application.
//

//#include "stdafx.h"
#include "UCommon.h"
#include "DX11.h"
#include <d3d11.h>
#include "FPSCounter.h"
#include "Mesh.h"
#include "FBXSDKMeshLoader.h"
#include <DirectXMath.h>
#include "Utility.h"
#include "PlaneMeshFactory.h"
#include "ShaderCompiler.h"
#include "Level.h"
#include "GameController.h"
#include "Time.h"
#include "GameWorld.h"

//debug
#include "FrameTes.h"
#include "Debug.h"

using namespace DirectX;
#define MAX_LOADSTRING 100

TCHAR *AppName = _T("DX11");

DWORD g_Width = 640, g_Height = 480;
//DWORD g_Width = 900, g_Height = 450;

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HRESULT InitScene();
//HRESULT InitResources();
//HRESULT InitResourcesFbx();
HRESULT GameLoop();
//HRESULT Render();


//定数バッファ
//[0] : WorldViewProjection変換行列
//[1] : ライティング（照明）
//ID3D11Buffer* g_pConstantBuffers[4] = { NULL, NULL, NULL, NULL };

//頂点バッファ
//ID3D11Buffer* g_pVertexBuffer;

//インデックスバッファ
//ID3D11Buffer* g_pIndexBuffer;

//頂点シェーダー
//ID3D11VertexShader* g_pVertexShader = NULL;

//ピクセルシェーダー
//ID3D11PixelShader* g_pPixelShader = NULL;

//シェーダーリソースビュー
//ID3D11ShaderResourceView *g_pSRView = NULL;

//サンプラーステート
//ID3D11SamplerState *g_pSamplerState = NULL;

//頂点レイアウト（入力レイアウト）
//　　DX11ではシェーダーの入力セマンティクスを自由に決めれられるようになったため
//　　頂点バッファとシェーダーを結びつけるために頂点レイアウトをデバイスに登録する
//ID3D11InputLayout* g_pLayout = NULL;

//自作DirectXメインクラス
D3D11User *g_pD3D11User = NULL;

//FBXをロードするクラス
FBXSDKMeshLoader *g_pMeshLoader = NULL;

//FBXから取り出されたメッシュ情報が格納されているクラス
//MeshGroup *g_pBoxMesh = NULL;

//ゲームコントローラーへのポインタ
GameController* g_pGameController = NULL;


//シーン
Level* g_pScene = NULL;

//ゲームワールド
GameWorld* g_pGameWorld = NULL;

//プリミティブ平面ポリゴン

//MeshGroup *g_pPlaneMesh = NULL;

//FPS計測クラス
FPSCounter* g_pFPSCounter = NULL;

//シェーダーをコンパイルする
ShaderCompiler* g_pShaderCompiler = NULL;

//テスト
FrameTes* g_pFrameTes = NULL;

//TCHAR *AppName = _T("DX11");

//UINT DrawMeshIndex = 0;

//平行光線ライト　（Directonal Light）　ワールド空間で定義
//XMFLOAT4 g_vecLight = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);

//環境光ライト　(0.0 ～ 1.0で指定）
//float g_Ambient = 0.0f;

//ランバート反射の定数バッファ構造体
//__declspec(align(16))
//struct CB_LAMBERT {
//	XMMATRIX matWVP;     //WVP変換行列
//	XMMATRIX matNormal;  //法線変換行列
//	XMFLOAT4 vecLight; //平行射影ライト
//	float ambient;        //環境光ライト
//						  //D3DMATRIX matWVP;     //WVP変換行列
//						  //D3DMATRIX matNormal;  //法線変換行列
//						  //D3DXVECTOR4 vecLight; //平行射影ライト
//						  //float ambient;        //環境光ライト
//};

//ID3D11Buffer* g_pConstantBuffersLambert = NULL;

//bool g_useLegacyCBSetMethod = false;

Time* g_pTime = nullptr;

//UINT g_frameTimeCount = 0;
//double g_Time = 0.0;
float g_deltaTime = 0.0;

//vertex format
// define
//#define D3DFVF_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

//定数バッファ構造体
//この構造体に定数バッファとして使用する値を格納し、DeviceContext->UpdateSubResouce関数でVRAM
//に転送する。この値とシェーダー内で定義する定数バッファは一致していなければならない
//struct ConstanBuffer {
//	XMMATRIX world; //ワールド変換行列
//	XMMATRIX view;  //ビュー変換行列
//	XMMATRIX proj;  //射影変換行列
//	XMMATRIX matWVP;  //WVP変換行列
//};

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DX11, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	// TODO: Place code here.
	//****************************************
	HRESULT hr = E_FAIL;

	//DirectXの初期化
	g_pD3D11User = new D3D11User();
	hr = g_pD3D11User->InitD3D11(hInst, hWnd);

	if (FAILED(hr))
	{
		OutputDebugString(_T("Error: Direct3D 11.0 Initialization failed"));
		return E_FAIL;
		//goto EXIT;
	}

	//ウィンドウを表示
	::ShowWindow(hWnd, SW_SHOW);
	::UpdateWindow(hWnd);

	//FBXSDKを使用して.fbxを読み込むクラスの初期化
	g_pMeshLoader = new FBXSDKMeshLoader();
	hr = g_pMeshLoader->Initialize(g_pD3D11User->m_D3DDevice);
	if (FAILED(hr))
	{
		OutputDebugString(_T("Error: FBXSDKMeshLoader Initialization failed"));
		return E_FAIL;
		//goto EXIT;
	}

	//使用するシェーダーのコンパイル
	g_pShaderCompiler = new ShaderCompiler();
	hr = g_pShaderCompiler->CompileShaders();
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : Shader Compile Failed...\n"));
		return E_FAIL;
	}

	//コントローラーの初期化
	g_pGameController = new GameController();
	g_pGameController->Initialize();

	//FPS計測クラスの初期化
	g_pFPSCounter = new FPSCounter(10);

	//ゲーム時間のクラス
	g_pTime = new Time();

	//シーンの初期化
	hr = InitScene();
	//****************************************

	//テスト用
	g_pFrameTes = new FrameTes();
	

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DX11));

    MSG msg;
	// Main message loop
	do {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			GameLoop();
		}

	} while (msg.message != WM_QUIT);
    
	// Main message loop
  //  while (GetMessage(&msg, nullptr, 0, 0))
  //  {
		////メニューの操作に対するキーボードからのメッセージを実際のメッセージに変換する
		////成功すると０以外、失敗すると０、成功したときはメッセージの処理を行ってはいけない
		////WM_SYSKEYDOWN や WM_KEYDOWN をWM_SYSCOMMAND や WM_COMMAND に変換する
  //      if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
  //      {
  //          TranslateMessage(&msg);
  //          DispatchMessage(&msg);
  //      }
		//GameLoop();
  //  }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX11));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DX11);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   //TODO: you should use DXGI_MODE_DESC to describe window size
   hWnd = CreateWindowW(
	   szWindowClass, 
	   szTitle,
	   WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 
	   0,
	   g_Width,//CW_USEDEFAULT,
	   g_Height,//0,
	   nullptr,
	   nullptr,
	   hInstance,
	   nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

HRESULT InitScene() 
{
	HRESULT hr = E_FAIL;

	//ゲームワールドインスタンスの作成
	//シーンの作成
	g_pGameWorld = new GameWorld();
	hr = g_pGameWorld->LoadLevel();
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : loading scene failed...\n"));
		goto EXIT;
	}
	hr = S_OK;

	//シーンの作成
	g_pScene = new Level();
	hr = g_pScene->LoadLevelSync();
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : loading scene failed...\n"));
		goto EXIT;
	}
	hr = S_OK;	

EXIT:
	return hr;
}

HRESULT GameLoop()
{
	//g_pFrameTes->time += 0.1f;
	//Debug::Log(_T("tes : %f\n"), g_pFrameTes->time);

	//レンダリング
	HRESULT hr = E_FAIL;

	//FPS計測クラスの更新
	g_pFPSCounter->GetFPS();

	//コントローラーの更新
	g_pGameController->Update();

	//フレーム時間の更新
	g_pTime->Update();

	//g_frameTimeCount++;
	//g_Time += 1.0f / 60.0f;
	//g_deltaTime = 1.0f / 60.0f;

	//float deltaTime = g_deltaTime;//(float)(g_pTime->m_deltaTime);
	//float z = 0;
	//z += deltaTime;
	//TCHAR debugStr[512];
	//_stprintf_s(debugStr, 512, _T("z [%f]\n"), z);
	//OutputDebugString(debugStr);

	//クライアント領域座標に変換
	//Debug::Log(_T("x: %f, y: %f\n"), p.x, p.y);

	//フレーム処理
	g_pGameWorld->UpdateGameWorld();

	//描画更新
	hr = g_pGameWorld->RenderGameWorld();

	return hr;
}

HRESULT Destroy() {

}


//HRESULT Render()
//{
//	HRESULT hr = E_FAIL;
//
//	//*** フレーム時間の更新 ***
//	g_frameTimeCount++;
//	g_Time += 1.0f / 60.0f;
//	//TCHAR debugStr[512];
//	//_stprintf_s(debugStr, 512, _T(" time [%f]\n"), g_Time);
//	//OutputDebugString(debugStr);
//	//*** モデルのアニメーションを更新 ***
//	//g_pBoxMesh->Meshes[0].UpdateAnimationTime();
//
//
//
//	//**** クリア処理 *****
//
//	//バックバッファをクリア
//	float clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
//	g_pD3D11User->m_D3DDeviceContext->ClearRenderTargetView(g_pD3D11User->m_RenderTargetView, clearColor);
//
//	//深度バッファをクリア
//	if (g_pD3D11User->m_DepthStencilView)
//		g_pD3D11User->m_D3DDeviceContext->ClearDepthStencilView(g_pD3D11User->m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//
//	//********* 頂点関連のデータをデバイスコンテキストにセット　**************
//
//
//	//頂点バッファを設定  --- 頂点バッファはそれぞれの頂点データ（座標など）の配列。描画するためには頂点バッファをデバイスに教える
//	UINT stride = sizeof(Mesh::GVERTEX);
//	UINT offset = 0;
//	g_pD3D11User->m_D3DDeviceContext->IASetVertexBuffers(0, 1, &g_pBoxMesh->Meshes[0].VertexBuffer, &stride, &offset);
//
//	//頂点インデックスバッファを設定 --- 頂点インデックスバッファはポリゴンを作るための頂点の結び順に頂点番号が格納されたバッファ	
//	g_pD3D11User->m_D3DDeviceContext->IASetIndexBuffer(g_pBoxMesh->Meshes[0].IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//
//	//頂点レイアウト設定
//	g_pD3D11User->m_D3DDeviceContext->IASetInputLayout(g_pLayout);
//
//	//プリミティブタイプおよびデータの順序に関する情報を設定 ---- 描画するプリミティブの形状を指定（ジオメトリシェーダーの場合シェーダー側で再定義されるので意味ないらしい）
//	//　*** フラットシェーディングの場合は、三角リストを指定
//	g_pD3D11User->m_D3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	//　*** グーローシェーディングを使う場合は、三角ストリップを指定
//	//g_pD3D11User->m_D3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//
//
//	//********** 行列設定 ****************************
//
//	//回転
//	static float rotX = 0.0f;
//	static float rotY = 0.0f;
//	static float rotZ = 0.0f;
//
//	//位置
//	static float transX = 0.0f;
//	static float transY = 0.0f;
//	static float transZ = 0.0f;
//
//	//スケール
//	static float scaleX = 5.0f;
//	static float scaleY = 5.0f;
//	static float scaleZ = 5.0f;
//
//	//フレーム移動
//	//rotY += 0.005f;
//
//	//ワールド、ビュー、プロジェクション変換行列, WVP最終変換行列
//	XMMATRIX matWorld, matView, matProj, matWVP, matNormal;
//	matWorld = XMMatrixIdentity();
//	matView = XMMatrixIdentity();
//	matProj = XMMatrixIdentity();
//	matWVP = XMMatrixIdentity();
//	matNormal = XMMatrixIdentity();
//
//	//射影変換行列 , fovY, aspect, Zn, Zf
//	{
//		matProj = XMMatrixPerspectiveFovLH(3.1415926f / 2.0f, 4.0f / 4.0f, 1.0f, 100.0f);
//	}
//
//	//ビュー変換行列
//	{
//		XMFLOAT3 eye = XMFLOAT3(0.0f, 0.0f, -100.0f);
//		XMFLOAT3 focus = XMFLOAT3(0.0f, 0.0f, 0.0f);
//		XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
//		matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&focus), XMLoadFloat3(&up));
//	}
//
//	//ワールド変換行列
//	{
//		XMMATRIX matIdentity, matScale, matTrans, matRot, matRotX, matRotY, matRotZ;
//		matScale = XMMatrixIdentity();
//		matTrans = XMMatrixIdentity();
//		matRot = XMMatrixIdentity();
//		matRotX = XMMatrixIdentity();
//		matRotY = XMMatrixIdentity();
//		matRotZ = XMMatrixIdentity();
//
//		//単位行列
//		matIdentity = XMMatrixIdentity();
//
//		//スケール変換
//		matScale = XMMatrixScaling(scaleX, scaleY, scaleZ);
//
//		//平行移動
//		matTrans = XMMatrixTranslation(transX, transY, transZ);
//
//		//回転
//		matRotX = XMMatrixRotationX(XMConvertToRadians(rotX));
//		matRotY = XMMatrixRotationY(XMConvertToRadians(rotY));
//		matRotZ = XMMatrixRotationZ(XMConvertToRadians(rotZ));
//		matRot = matRotX * matRotY * matRotZ;
//
//		matWorld = matIdentity * matScale * matRot * matTrans;
//
//		//計算を行う場合、平行移動に関しては無視、回転行列に関してはそのまま使用できる、スケール行列に関してはxが3倍なら1/3倍のように逆数を使う
//		//これらを考慮すると以下のように求められる
//		matNormal = matIdentity * matRot;
//	}
//	//ワールド・ビュー・射影変換行列を作成
//	matWVP = matWorld * matView * matProj;
//
//	//D3DXMATRIXは行優先の行列、行優先というのは行単位でひとつの定数レジスタに格納するということ
//	//しかしシェーダー内では列優先のレイアウトで配置しているから転置して列と行を入れ替える必要がある
//	//シェーダーでも扱う行列をすべて入れ替える
//	matNormal = XMMatrixTranspose(matNormal);//法線変換行列
//	matWVP = XMMatrixTranspose(matWVP);//WVP行列
//
//	//**********　定数バッファ ****************************
//
//	//行列を定数バッファに設定、方法が２つある
//	//①直接コピーする方法(CopyMemory関数で直接使う、D3DDeviceContext->MapとUnMapを使用してメモリアクセスを制限）
//	if (g_useLegacyCBSetMethod)
//	{
//		/*
//		D3D11_MAPPED_SUBRESOURCE mappedResource;
//		//定数バッファ構造体
//		CB_LAMBERT* cbLambert;
//
//		// MVP変換行列用の定数バッファ
//		{
//		// サブリソースに格納されているデータへのポインターを取得して、そのサブリソースへの GPU のアクセスを拒否。
//		//これはメモリのLockなどと似ている
//		hr = g_pD3D11User->m_D3DDeviceContext->Map(g_pConstantBuffers[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//		if (FAILED(hr)) goto EXIT;
//
//		::CopyMemory(mappedResource.pData, &matWVP, sizeof(D3DXMATRIX));
//
//		//サブリソースへのGPUのアクセスを可能にするように戻す
//		//これはメモリのUnlockなどと似ている
//		g_pD3D11User->m_D3DDeviceContext->Unmap(g_pConstantBuffers[0], 0);
//		}
//
//
//		// ライティング用の定数バッファ
//		{
//		hr = g_pD3D11User->m_D3DDeviceContext->Map(g_pConstantBuffers[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//		if (FAILED(hr)) goto EXIT;
//
//		//このライトはワールド空間を基準に作成
//		//ライトにも同様にMVP行列変換が必要
//		//シェーダー内で扱うライトは
//		D3DXVec3Normalize((D3DXVECTOR3*)&g_vecLight, (D3DXVECTOR3*)&g_vecLight);
//		::CopyMemory(mappedResource.pData, &g_vecLight, sizeof(D3DXVECTOR4));
//
//		//サブリソースへのGPUのアクセスを可能にするように戻す
//		//これはメモリのUnlockなどと似ている
//		g_pD3D11User->m_D3DDeviceContext->Unmap(g_pConstantBuffers[1], 0);
//		}
//
//		// 環境光の定数バッファ
//		{
//		hr = g_pD3D11User->m_D3DDeviceContext->Map(g_pConstantBuffers[2], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//		if (FAILED(hr)) goto EXIT;
//
//		//環境光を設定
//		::CopyMemory(mappedResource.pData, &g_Ambient, sizeof(float));
//
//		//サブリソースへのGPUのアクセスを可能にするように戻す
//		//これはメモリのUnlockなどと似ている
//		g_pD3D11User->m_D3DDeviceContext->Unmap(g_pConstantBuffers[2], 0);
//		}
//
//		// 法線をローカル座標からワールド座標に変換するための行列定数バッファ
//		{
//		hr = g_pD3D11User->m_D3DDeviceContext->Map(g_pConstantBuffers[3], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//		if (FAILED(hr)) goto EXIT;
//
//		//法線変換用行列を設定
//		::CopyMemory(mappedResource.pData, &matNormal, sizeof(D3DXMATRIX));
//
//		//サブリソースへのGPUのアクセスを可能にするように戻す
//		//これはメモリのUnlockなどと似ている
//		g_pD3D11User->m_D3DDeviceContext->Unmap(g_pConstantBuffers[3], 0);
//		}
//		*/
//	}
//
//	//②UpdateSubresourceを使う方法
//	//常にこれを使おうとするとこのエラーがでる→Can only invoke UpdateSubresource when the destination Resource was created with D3D11_USAGE_DEFAULT and is not a multisampled Resource.
//	//定数バッファ構造体に値を設定
//	else
//	{
//		CB_LAMBERT cb;
//		cb.matWVP = matWVP;
//		cb.matNormal = matNormal;
//		cb.vecLight = g_vecLight;
//		cb.ambient = g_Ambient;
//
//		//定数バッファ構造体の情報をVRAM上に転送
//		g_pD3D11User->m_D3DDeviceContext->UpdateSubresource(g_pConstantBuffersLambert, 0, NULL, &cb, 0, 0);
//	}
//
//	// ************ シェーダー関連 ************************
//
//	//頂点シェーダー
//	{
//		//頂点シェーダーをデバイスに設定 (頂点シェーダーへのポインタ, シェーダーでインターフェイスを使用する場合必要, シェーダーのインターフェイスのクラスインスタンス数)
//		g_pD3D11User->m_D3DDeviceContext->VSSetShader(g_pVertexShader, NULL, 0);
//
//		//頂点シェーダーに定数バッファを設定 (デバイスの配列の中で定数バッファの設定を開始する位置, 設定するバッファの数, デバイスに設定する定数バッファ)
//		{
//			if (g_useLegacyCBSetMethod)
//			{
//				//MVP行列
//				g_pD3D11User->m_D3DDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffers[0]);
//
//				//平行射影ライト（グーローシェーディングでは頂点でライト情報を使うから頂点シェーダーの定数バッファとして設定）
//				g_pD3D11User->m_D3DDeviceContext->VSSetConstantBuffers(1, 1, &g_pConstantBuffers[1]);
//
//				//環境光ライト
//				g_pD3D11User->m_D3DDeviceContext->VSSetConstantBuffers(2, 1, &g_pConstantBuffers[2]);
//
//				//法線変換用行列を設定
//				g_pD3D11User->m_D3DDeviceContext->VSSetConstantBuffers(3, 1, &g_pConstantBuffers[3]);
//			}
//			else
//			{
//				//ランバート反射の定数バッファ構造体の定数バッファを頂点シェーダーにセット
//				g_pD3D11User->m_D3DDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffersLambert);
//			}
//		}
//	}
//
//	//ピクセルシェーダー
//	{
//		//ピクセルシェーダーをデバイスに設定
//		g_pD3D11User->m_D3DDeviceContext->PSSetShader(g_pPixelShader, NULL, 0);
//
//		//ピクセルシェーダーに定数バッファを設定 (ピクセル シェーダーのパイプライン ステージで使用される定数バッファーを設定)
//
//		//フォンシェーディングでは使用する
//		//g_pD3D11User->m_D3DDeviceContext->PSSetConstantBuffers(1, 1, &g_pConstantBuffers[1]);
//
//		//シェーダーにサンプラーステートを設定
//		g_pD3D11User->m_D3DDeviceContext->PSSetSamplers(0, 1, &g_pSamplerState);
//
//		//シェーダーにシェーダーリソースビューを設定（シェーダー用にロードしたテクスチャ）
//
//		//これは描画されるメッシュで使用されるテクスチャを設定する
//		ID3D11ShaderResourceView* pSRV = NULL;
//
//		//メッシュで使用されているテクスチャを取得
//		//TODO: もし複数のテクスチャを使っている場合全部
//		g_pBoxMesh->Meshes[DrawMeshIndex].GetTexture(_T("DiffuseColor"), &pSRV);
//
//		//ピクセルシェーダーにテクスチャリソースビューを設定
//		g_pD3D11User->m_D3DDeviceContext->PSSetShaderResources(0, 1, &pSRV);
//
//		//デバッグ用
//		//g_pD3D11User->m_D3DDeviceContext->PSSetShaderResources(0, 1, &g_pSRView);
//	}
//
//	//ハルシェーダーを無効にする
//	g_pD3D11User->m_D3DDeviceContext->HSSetShader(NULL, NULL, 0);
//
//	//ドメインシェーダーを無効にする
//	g_pD3D11User->m_D3DDeviceContext->DSSetShader(NULL, NULL, 0);
//
//	//ジオメトリシェーダーを無効にする
//	g_pD3D11User->m_D3DDeviceContext->GSSetShader(NULL, NULL, 0);
//
//	//コンピュートシェーダーを無効にする
//	g_pD3D11User->m_D3DDeviceContext->CSSetShader(NULL, NULL, 0);
//
//
//	//両面描画に変更する処理
//
//
//	//ラスタライザの設定を取得
//	ID3D11RasterizerState* pRasterState = NULL;
//	g_pD3D11User->m_D3DDeviceContext->RSGetState(&pRasterState);
//	//※注意　RSGetStateはデフォルトでは未設定でNULLが設定されている。RSGetStateでNULLが返ってきてしまうので、よって初期化段階で初期値を設定しておくこと
//
//	D3D11_RASTERIZER_DESC RasterDesc;
//	//::ZeroMemory(&RasterDesc, sizeof(D3D11_RASTERIZER_DESC));
//	pRasterState->GetDesc(&RasterDesc);
//
//	//取得したラスタライザの設定を両面描画に変更して再設定
//	//RasterDesc.CullMode = D3D11_CULL_NONE;
//
//	g_pD3D11User->m_D3DDevice->CreateRasterizerState(&RasterDesc, &pRasterState);
//	g_pD3D11User->m_D3DDeviceContext->RSSetState(pRasterState);
//
//	SAFE_RELEASE(pRasterState);
//
//
//	// ************ 描画　************************
//
//	//インデックスバッファを使用した描画
//	g_pD3D11User->m_D3DDeviceContext->DrawIndexed(g_pBoxMesh->Meshes[DrawMeshIndex].IndexCount, 0, 0);
//
//	//画面に表示　TODO: 関数の詳細を検索が必要
//	hr = g_pD3D11User->m_SwapChain->Present(0, 0);
//
//	//終了処理
//EXIT:
//	//SAFE_RELEASE(constantBuffers);
//
//	return hr;
//
//}

/*
HRESULT InitResourcesFbx()
{
	HRESULT hr = E_FAIL;

	// ****** FBXでモデルの読み込み ******

	// 頂点情報を取得する
	//hr = g_pMeshLoader->LoadMeshData(_T("cube.fbx"), &g_pBoxMesh);
	hr = g_pMeshLoader->LoadMeshData(_T("C:/Users/yuya/Desktop/TestProjects/DX11/Resources/bone.fbx"), &g_pBoxMesh);
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : loading fbx failed...\n"));
		goto EXIT;
	}

	//一番最初のアニメーションをセット
	g_pBoxMesh->Meshes[0].SetAnimation(0);

	//地形平面ポリゴンの生成
	PlaneMeshFactory* lPlaneMeshFactory = new PlaneMeshFactory();
	hr = lPlaneMeshFactory->CreateMesh(&g_pPlaneMesh);
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : loading fbx failed...\n"));
		goto EXIT;
	}

	// *** シェーダーのコンパイル

	g_pShaderCompiler = new ShaderCompiler();
	hr = g_pShaderCompiler->CompileShaders();
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : Shader Compile Failed...\n"));
		goto EXIT;
	}

	// ******** 頂点シェーダーを作成 *********** //

	//頂点要素の記述（頂点構造体側セマンティクス）
	//　　　頂点構造体に宣言されている項目の種類、サイズを記述するD3D11_INPUT_ELEMENT_DESC構造体
	D3D11_INPUT_ELEMENT_DESC layout[] = {

		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//自作DX11クラスで頂点シェーダー作成
	hr = g_pD3D11User->CreateVertexShaderFromFile(
		&g_pVertexShader,  //頂点シェーダーオブジェクトへのポインタ
		_T("BasicHLSLFbx.hlsl"),       //シェーダーコードが格納されているファイル名（プロジェクトルートからのパスで指定）
		"VS_Main",         //シェーダーの実行が開始されるエントリポイント関数名
		"vs_4_0",          //シェーダーモデルを指定
		&g_pLayout,        //頂点レイアウトへのポインタ
		layout,            //頂点レイアウトを作るための頂点情報(D3D11_INPUT_ELEMENT_DESC)
		_countof(layout)   //頂点情報配列数
	);
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : Create vertex shader from file failed\n"));
		goto EXIT;
	}
	// ******** ピクセルシェーダーを作成 *********** //

	hr = g_pD3D11User->CreatePixelShaderFromFile(
		&g_pPixelShader,   //ピクセルシェーダーオブジェクトへのポインタ
		_T("BasicHLSLFbx.hlsl"),       //シェーダーコードが格納されているファイル名（プロジェクトルートからのパスで指定）
		"PS_Main",         //シェーダーの実行が開始されるエントリポイント関数名
		"ps_4_0"           //シェーダーモデルを指定
	);
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : Create pixel shader from file failed\n"));
		goto EXIT;
	}

	//**** 定数バッファを作成 **************************

	//ランバート反射用定数バッファ構造体の定数バッファを作成
	//CPUAccessFlagの設定は
	hr = g_pD3D11User->CreateConstantBuffer(&g_pConstantBuffersLambert, NULL, sizeof(CB_LAMBERT), 0);
	if (FAILED(hr))
		goto EXIT;


	// ****** サンプラーステートの設定 *****************
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;         //サンプリング時に使用するフィルターを指定。これは異方性フィルター
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;     //0－１範囲外にあるuテクスチャー座標描画法。これはラッピング設定。タイルなどもある
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;     //0－１範囲外にあるvテクスチャー座標描画法。これはラッピング設定
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;     //0－１範囲外にあるwテクスチャー座標描画法。これはラッピング設定
	samplerDesc.MipLODBias = 0;                            //計算されたミップマップレベルからのバイアス
	samplerDesc.MaxAnisotropy = 16;                        //サンプリングに異方性フィルタリングを使用している場合の限界値。1-16で指定
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  //比較オプション
														   //::CopyMemory(samplerDesc.BorderColor, & Utility::Co D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f), sizeof(D3DXVECTOR4)); //境界色
	samplerDesc.MinLOD = 0;                                //アクセス可能なミップマップ下限値
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                //アクセス可能なミップマップ上限値
														   //サンプラーステートの作成
	hr = g_pD3D11User->m_D3DDevice->CreateSamplerState(&samplerDesc, &g_pSamplerState);
	if (FAILED(hr)) {
		OutputDebugString(_T("error : Sampler state create failed .... \n"));
		goto EXIT;
	}

	OutputDebugString(_T("Resource Initialization succeeded! ******************** \n"));
	hr = S_OK;

	//終了処理
EXIT:
	SAFE_DELETE(lPlaneMeshFactory);

	return hr;
}
*/
/*
HRESULT InitResources()
{
	HRESULT hr = E_FAIL;

	// ******** 頂点シェーダーを作成 *********** //

	//頂点要素の記述（頂点構造体側セマンティクス）
	//　　　頂点構造体に宣言されている項目の種類、サイズを記述するD3D11_INPUT_ELEMENT_DESC構造体
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	TCHAR vsShaderFileName[512] = _T("BasicHLSLFbx.hlsl");
	const CHAR* vsFunctionName = "VS_Main";
	const CHAR* vsShaderModel = "vs_5_0";
	//自作DX11クラスで頂点シェーダー作成
	hr = g_pD3D11User->CreateVertexShaderFromFile(
		&g_pVertexShader,  //頂点シェーダーオブジェクトへのポインタ
		vsShaderFileName,       //シェーダーコードが格納されているファイル名（プロジェクトルートからのパスで指定）
		vsFunctionName,         //シェーダーの実行が開始されるエントリポイント関数名
		vsShaderModel,          //シェーダーモデルを指定
		&g_pLayout,        //頂点レイアウトへのポインタ
		layout,            //頂点レイアウトを作るための頂点情報(D3D11_INPUT_ELEMENT_DESC)
		_countof(layout)   //頂点情報配列数
	);
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : Create vertex shader from file failed\n"));
		return E_FAIL;
		//goto EXIT;
	}
	// ******** ピクセルシェーダーを作成 *********** //
	TCHAR psShaderFileName[512] = _T("BasicHLSLFbx.hlsl");
	const CHAR* psFunctionName = "PS_Main";
	const CHAR* psShaderModel = "ps_5_0";
	hr = g_pD3D11User->CreatePixelShaderFromFile(
		&g_pPixelShader,   //ピクセルシェーダーオブジェクトへのポインタ
		psShaderFileName,       //シェーダーコードが格納されているファイル名（プロジェクトルートからのパスで指定）
		psFunctionName,         //シェーダーの実行が開始されるエントリポイント関数名
		psShaderModel           //シェーダーモデルを指定
	);
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : Create pixel shader from file failed\n"));
		goto EXIT;
	}


	//終了処理
EXIT:

	return hr;
}
*/





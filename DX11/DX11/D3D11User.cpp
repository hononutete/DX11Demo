//#include "stdafx.h"
//#include "UCommon.h"
#include "D3D11User.h"
#include <dxgi.h>
#include <d3dcompiler.h>
//#include <D3DX11async.h>

D3D11User::D3D11User()
{
	m_D3DDevice = NULL;
	m_Adapter = NULL;
	m_D3DDeviceContext = NULL;
	m_SwapChain = NULL;
	m_RenderTargetView = NULL;
	m_DepthStencilView = NULL;
	//m_hWnd = NULL;
	
}


D3D11User::~D3D11User()
{
	Invalidate();
}
HRESULT D3D11User::InitD3D11(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = E_FAIL;

	//ウィンドウの作成
	//hr = InitWindow(WinProc, hInstance, AppName);
	//if (FAILED(hr)) goto EXIT;

	//アダプター作成
	hr = CreateAdapter();
	if (FAILED(hr)) goto EXIT;

	//デバイスの作成（描画機能）
	hr = CreateDevice();
	if (FAILED(hr)) goto EXIT;

	//スワップチェーン作成（バッファリソースを管理）
	hr = CreateSwapChain(hWnd);
	if (FAILED(hr)) goto EXIT;

	//レンダーターゲットビューを作成
	hr = CreateRenderTargetView();
	if (FAILED(hr)) goto EXIT;

	//深度ステンシルビューの作成
	hr = CreateDepthStencilView();
	if (FAILED(hr)) goto EXIT;

	//デバイスとレンダーターゲットビューと深度ステンシルバッファーの紐付け
	m_D3DDeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, NULL);

	//ビューポート設定
	hr = CreateViewport();
	if (FAILED(hr)) goto EXIT;

	//デフォルトのラスタライザ設定
	hr = SetDefaultRasterize();
	if (FAILED(hr)) goto EXIT;

	//デフォルトの深度ステンシルステートの設定
	hr = SetDefaultDepthStencilState();
	if (FAILED(hr)) goto EXIT;

	OutputDebugString(_T("D3D Initialization Completed!\n"));

	hr = S_OK;

EXIT:
	return hr;
}


HRESULT D3D11User::InitD3D11(WNDPROC WinProc, HINSTANCE hInstance, TCHAR* AppName)
{
	HRESULT hr = E_FAIL;

	//ウィンドウの作成
	hr = InitWindow(WinProc, hInstance, AppName);
	if (FAILED(hr)) goto EXIT;

	//アダプター作成
	hr = CreateAdapter();
	if (FAILED(hr)) goto EXIT;

	//デバイスの作成（描画機能）
	hr = CreateDevice();
	if (FAILED(hr)) goto EXIT;

	//スワップチェーン作成（バッファリソースを管理）
	hr = CreateSwapChain();
	if (FAILED(hr)) goto EXIT;

	//レンダーターゲットビューを作成
	hr = CreateRenderTargetView();
	if (FAILED(hr)) goto EXIT;

	//深度ステンシルビューの作成
	hr = CreateDepthStencilView();
	if (FAILED(hr)) goto EXIT;

	//デバイスとレンダーターゲットビューと深度ステンシルバッファーの紐付け
	m_D3DDeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, NULL);

	//ビューポート設定
	hr = CreateViewport();
	if (FAILED(hr)) goto EXIT;

	//デフォルトのラスタライザ設定
	hr = SetDefaultRasterize();
	if (FAILED(hr)) goto EXIT;

	//デフォルトの深度ステンシルステートの設定
	hr = SetDefaultDepthStencilState();
	if (FAILED(hr)) goto EXIT;

	OutputDebugString(_T("D3D Initialization Completed!\n"));

	hr = S_OK;

EXIT:
	return hr;
}

void D3D11User::Invalidate()
{
	ULONG cnt = 0;

	//レンダーターゲットビューのメモリ解放
	if (m_RenderTargetView)
	{
		cnt = m_RenderTargetView->Release();
		if (cnt != 0) OutputDebugString(_T("RenderTargetView reference counter invalid\n"));
		m_RenderTargetView = NULL;
	}

	if (m_SwapChain)
	{
		m_SwapChain->Release();
		if (cnt != 0) OutputDebugString(_T("SwapChain reference counter invalid\n"));
		m_SwapChain = NULL;
	}

	if (m_D3DDeviceContext)
	{
		m_D3DDeviceContext->Release();
		if (cnt != 0) OutputDebugString(_T("DeviceContext reference counter invalid\n"));
		m_D3DDeviceContext = NULL;
	}

	if (m_D3DDevice)
	{
		m_D3DDevice->Release();
		if (cnt != 0) OutputDebugString(_T("Device reference counter invalid\n"));
		m_D3DDevice = NULL;
	}

	if (m_SwapChain)
	{
		m_SwapChain->Release();
		if (cnt != 0) OutputDebugString(_T("RenderTargetView reference counter invalid\n"));
		m_SwapChain = NULL;
	}

	if (m_DepthStencilView)
	{
		m_DepthStencilView->Release();
		if (cnt != 0) OutputDebugString(_T("DepthStencilView reference counter invalid\n"));
		m_DepthStencilView = NULL;
	}

	if (m_Adapter)
	{
		m_Adapter->Release();
		if (cnt != 0) OutputDebugString(_T("Adapter reference counter invalid\n"));
		m_Adapter = NULL;
	}
}

HRESULT D3D11User::InitWindow(WNDPROC WinProc, HINSTANCE hInstance, TCHAR* AppName)
{
	HRESULT hr = E_FAIL;

//	//ウィンドウクラスの作成
//	WNDCLASSEX wc;
//	wc.cbSize = sizeof(WNDCLASSEX);
//	wc.style = CS_VREDRAW | CS_HREDRAW;
//	wc.lpfnWndProc = WinProc;
//	wc.cbClsExtra = 0;
//	wc.cbWndExtra = sizeof(DWORD);
//	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
//	wc.hIcon = NULL;
//	wc.hIconSm = NULL;
//	wc.lpszMenuName = NULL;
//	wc.lpszClassName = AppName;
//	wc.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
//	wc.hInstance = hInstance;
//
//	//ウィンドウクラスの登録
//	RegisterClassEx(&wc);
//
//	//ウィンドウの作成
//	m_hWnd = ::CreateWindow(
//		wc.lpszClassName,
//		wc.lpszClassName,
//		WS_OVERLAPPEDWINDOW,
//		100,
//		100,
//		300,
//		300,
//		NULL,
//		NULL,
//		hInstance,
//		NULL
//		);
//
//	if (m_hWnd == NULL)
//	{
//		OutputDebugString(_T("Error: Window Creation Failed\n"));
//		hr = E_FAIL;
//		goto EXIT;
//	}
//
//	OutputDebugString(_T("Window Creation Succeeded!\n"));
//	hr = S_OK;
//EXIT:
//	
	return hr;
}

HRESULT D3D11User::CreateAdapter()
{
	HRESULT hr = E_FAIL;
	IDXGIFactory* pFactory = NULL;

	if (m_Adapter == NULL)
	{
		//ファクトリの作成（デフォルトアダプターを取得するのに必要）
		hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));
		if (FAILED(hr)) goto EXIT;

		//デフォルトアダプターを取得
		hr = pFactory->EnumAdapters(0, &m_Adapter);
		if (FAILED(hr)) goto EXIT;
	}

	OutputDebugString(_T("Adapter Creation Succeeded!\n"));
	hr = S_OK;

	//終了処理
EXIT:
	SAFE_RELEASE( pFactory );
	
	return hr;
}

HRESULT D3D11User::CreateDevice()
{
	//それぞれのバージョンでサポートされている機能をターゲットとする
	D3D_FEATURE_LEVEL FeatureLevel[] = {
		D3D_FEATURE_LEVEL_11_0,  // Direct3D 11.0 ShaderModel 5
		D3D_FEATURE_LEVEL_10_1,  // Direct3D 10.1 SM 4
		D3D_FEATURE_LEVEL_10_0,  // Direct3D 10.0 SM 4
		D3D_FEATURE_LEVEL_9_3,   // Direct3D 9.3  SM 3
		D3D_FEATURE_LEVEL_9_2,   // Direct3D 9.2  SM 2
		D3D_FEATURE_LEVEL_9_1,   // Direct3D 9.1  SM 2
	};

	//D3D_FEATURE_LEVEL配列の要素数
	int featureCnt = sizeof(FeatureLevel) / sizeof(D3D_FEATURE_LEVEL);

	//目指すべき引数
	HRESULT hr = D3D11CreateDevice(
		m_Adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		FeatureLevel,
		featureCnt,
		D3D11_SDK_VERSION,
		&m_D3DDevice,
		&m_FeatureLevel,
		&m_D3DDeviceContext
		);

	if (FAILED (hr))
	{
		OutputDebugString(_T("Error : Device Creation Failed\n"));
		return E_FAIL;
	}

	OutputDebugString(_T("Device Creation Succeeded!\n"));

	return S_OK;
	
}

HRESULT D3D11User::CreateSwapChain()
{
//	HRESULT hr = E_FAIL;
//
//	//DXGI_SWAP_CHAIN_DESC構造体（スワップチェインの能力を指定）
//	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
//	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
//	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //バックバッファフォーマット
//	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //バックバッファのサーフェス使用法および CPU アクセス オプションを表す DXGI_USAGE 列挙型のメンバー。バックバッファにレンダリング可能にする
//	SwapChainDesc.BufferCount = 1; //スワップチェーンのバッファの数
//	SwapChainDesc.OutputWindow = m_hWnd; //出力ウィンドウへのHWNDハンドル
//	SwapChainDesc.Windowed = TRUE;
//	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //サーフェスのスワップ処理後にバックバッファの内容を保持するか？バックバッファの内容を保持しない
//	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //スワップチェーンの付加能力を指定。フルスクリーンとウィンドウの解像度変更が自動で行ってくれる
//
//	//マルチサンプリングの設定（アンチエイリアス）
//	SwapChainDesc.SampleDesc.Count = 1; //１ピクセルの色を決めるためのサンプリング数
//	SwapChainDesc.SampleDesc.Quality = 0; //精度レベル
//
//	IDXGIFactory* pFactory;
//	//IDXGIFactoryは、フルスクリーン切り替えを処理する DXGI オブジェクト生成メソッドを実装
//	//IDXGIFactoryを使ってスワップチェーンを作成する
//	//CreateDXGIFactoryでDXGIFactoryを生成した場合，以降のIDXGIFactory1，2，3，4のインターフェースをサポートしたオブジェクトにはならない
//	//hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));
//
//	//IDXGIAdapterから作成
//	hr = m_Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
//
//	if (FAILED(hr))
//	{
//		OutputDebugString(_T("Error: IDXGIFactory Creation Failed\n"));
//		goto EXIT;
//	}
//
//	//スワップチェーンをIDXGIFactoryを使って生成する
//	hr = pFactory->CreateSwapChain(m_D3DDevice, &SwapChainDesc, &m_SwapChain);
//
//	if (FAILED(hr))
//	{
//		OutputDebugString(_T("Error: SwapChain Creation Failed\n"));
//		goto EXIT;
//	}
//
//	OutputDebugString(_T("SwapChain Creation Succeeded!\n"));
//
//	hr = S_OK;
//
//	//終了処理
//EXIT:
//	SAFE_RELEASE(pFactory);
//
//	return hr;
	return E_FAIL;
}

HRESULT D3D11User::CreateSwapChain(HWND hWnd) {
	HRESULT hr = E_FAIL;

	//DXGI_SWAP_CHAIN_DESC構造体（スワップチェインの能力を指定）
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //バックバッファフォーマット
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //バックバッファのサーフェス使用法および CPU アクセス オプションを表す DXGI_USAGE 列挙型のメンバー。バックバッファにレンダリング可能にする
	SwapChainDesc.BufferCount = 1; //スワップチェーンのバッファの数
	SwapChainDesc.OutputWindow = hWnd; //出力ウィンドウへのHWNDハンドル
	SwapChainDesc.Windowed = TRUE;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //サーフェスのスワップ処理後にバックバッファの内容を保持するか？バックバッファの内容を保持しない
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //スワップチェーンの付加能力を指定。フルスクリーンとウィンドウの解像度変更が自動で行ってくれる

																  //マルチサンプリングの設定（アンチエイリアス）
	SwapChainDesc.SampleDesc.Count = 1; //１ピクセルの色を決めるためのサンプリング数
	SwapChainDesc.SampleDesc.Quality = 0; //精度レベル

	IDXGIFactory* pFactory;
	//IDXGIFactoryは、フルスクリーン切り替えを処理する DXGI オブジェクト生成メソッドを実装
	//IDXGIFactoryを使ってスワップチェーンを作成する
	//CreateDXGIFactoryでDXGIFactoryを生成した場合，以降のIDXGIFactory1，2，3，4のインターフェースをサポートしたオブジェクトにはならない
	//hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));

	//IDXGIAdapterから作成
	hr = m_Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	if (FAILED(hr))
	{
		OutputDebugString(_T("Error: IDXGIFactory Creation Failed\n"));
		goto EXIT;
	}

	//スワップチェーンをIDXGIFactoryを使って生成する
	hr = pFactory->CreateSwapChain(m_D3DDevice, &SwapChainDesc, &m_SwapChain);

	if (FAILED(hr))
	{
		OutputDebugString(_T("Error: SwapChain Creation Failed\n"));
		goto EXIT;
	}

	OutputDebugString(_T("SwapChain Creation Succeeded!\n"));

	hr = S_OK;

	//終了処理
EXIT:
	SAFE_RELEASE(pFactory);

	return hr;

}
//深度ステンシルビューの作成
//深度バッファとステンシルバッファを同時に作成
HRESULT D3D11User::CreateDepthStencilView()
{
	HRESULT hr = E_FAIL;
	ID3D11Texture2D *pDepthBuffer = NULL; //深度バッファ用テクスチャ

	//スワップチェーンの情報を取得（深度ステンシルバッファのサイズなどはバックバッファと同じでなければいけない）
	DXGI_SWAP_CHAIN_DESC chainDesc;
	hr = m_SwapChain->GetDesc(&chainDesc);

	//深度バッファ用テクスチャ設定の構造体
	D3D11_TEXTURE2D_DESC descDS; 
	::ZeroMemory(&descDS, sizeof(D3D11_TEXTURE2D_DESC));
	descDS.Width              = chainDesc.BufferDesc.Width;    //バックバッファと同じ
	descDS.Height             = chainDesc.BufferDesc.Height;   //バックバッファと同じ
	descDS.MipLevels          = 1;                             //ミップマップは作成しない
	descDS.ArraySize          = 1;                             //テクスチャの配列数
	descDS.Format             = DXGI_FORMAT_R16_TYPELESS;      //フォーマット 
	descDS.SampleDesc.Count   = chainDesc.SampleDesc.Count;    //バックバッファと同じ
	descDS.SampleDesc.Quality = chainDesc.SampleDesc.Quality;  //バックバッファと同じ
	descDS.Usage              = D3D11_USAGE_DEFAULT;           //GPUによる読み取りおよび書き込みアクセスを必要とするリソース
	descDS.BindFlags          = D3D11_BIND_DEPTH_STENCIL |     //深度ステンシルバッファとして作成
		                        D3D11_BIND_SHADER_RESOURCE;    //シェーダーリソースビューとして作成
	descDS.CPUAccessFlags     = 0;                             //CPUアクセスは不要
	descDS.MiscFlags          = 0;                             //その他のフラグも設定しない
	
	//深度バッファ用のテクスチャー作成
	hr = m_D3DDevice->CreateTexture2D(&descDS, NULL, &pDepthBuffer);
	if (FAILED(hr)){
		OutputDebugString(_T("DepthStencil surface texture creation failed...\n"));
		goto EXIT;
	}
	


	//深度ステンシルビューの設定の構造体。深度ステンシルビューを作る時に
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	::ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	descDSV.Format             = DXGI_FORMAT_D16_UNORM; //テクスチャのフォーマットと互換性があるフォーマットを指定
	descDSV.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D; //マルチサンプルを使用しない場合（使用する場合はD3D11_DSV_DIMENSION_TEXTURE2DMS）
	descDSV.Texture2D.MipSlice = 0;

	//深度ステンシルビューの作成
	hr = m_D3DDevice->CreateDepthStencilView(pDepthBuffer, &descDSV, &m_DepthStencilView);
	if (FAILED(hr)){
		OutputDebugString(_T("DepthStencilView creation failed...\n"));
		goto EXIT;
	}

	hr = S_OK;
	OutputDebugString(_T("DepthStencilView creation succeeded!\n"));

	//終了処理
EXIT:
	SAFE_RELEASE(pDepthBuffer);

	return hr;

}

//デフォルトの深度ステンシルステートを設定
HRESULT D3D11User::SetDefaultDepthStencilState()
{
	HRESULT hr = E_FAIL;

	//深度ステンシルステートインターフェイス
	ID3D11DepthStencilState *pDepthStencilState = NULL;

	//深度ステンシルステート構造体
	//この構造体では、深度バッファとステンシルバッファの使用方法を制御する
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	::ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable    = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc      = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable  = FALSE;

	//深度ステンシルステートを作成
	hr = m_D3DDevice->CreateDepthStencilState(&dsDesc, &pDepthStencilState);
	if (FAILED(hr)){
		OutputDebugString(_T("DepthStencilState creation failed...\n"));
		goto EXIT;
	}

	//深度ステンシルステートインターフェイスをコンテキストに設定
	//第二引数は基準として使用される参照値
	m_D3DDeviceContext->OMSetDepthStencilState( pDepthStencilState, 0);

	hr = S_OK;
	OutputDebugString(_T("DepthStencilState creation succeeded!\n"));

	//終了処理
EXIT:
	SAFE_RELEASE(pDepthStencilState);

	return hr;
}

HRESULT D3D11User::SetDefaultRasterize()
{
	HRESULT hr = E_FAIL;

	//ラスタライザーステージのラスタライザーステートにアクセスするためのインターフェイス
	ID3D11RasterizerState* g_pRasterState = NULL;

	//ラスタライザー設定を行う構造体
	D3D11_RASTERIZER_DESC rsState;
	rsState.FillMode = D3D11_FILL_SOLID; //ポリゴン面描画
	rsState.CullMode = D3D11_CULL_BACK;  //裏面を描画しない
	//rsState.FrontCounterClockwise = FALSE; //時計回りを表面とする
	rsState.FrontCounterClockwise = TRUE; //反時計回りを表面とする
	rsState.DepthBias = 0;
	rsState.DepthBiasClamp = 0;
	rsState.SlopeScaledDepthBias = 0;
	rsState.DepthClipEnable = TRUE;
	rsState.ScissorEnable = FALSE; //シザー矩形無効

	//ラスタライザーのマルチサンプリングの設定は、スワップチェーンのマルチサンプリング設定に合わせる
	DXGI_SWAP_CHAIN_DESC swapDesc;
	m_SwapChain->GetDesc(&swapDesc);
	if (swapDesc.SampleDesc.Count != 1)
		rsState.MultisampleEnable = TRUE;
	else
		rsState.MultisampleEnable = FALSE;

	rsState.AntialiasedLineEnable = FALSE; //アンチエイリアスの設定


	//ラスタライザ設定構造体から、ラスタライザーステートインターフェイスを作成する。
	hr = m_D3DDevice->CreateRasterizerState(&rsState, &g_pRasterState);
	if (FAILED(hr)){
		OutputDebugString(_T("error: Default Rasterize State Creation failed...\n"));
		goto EXIT;
	}

	//デバイスコンテキストにラスタライザーステートを設定する。これが初期ラスタライザ設定になる
	m_D3DDeviceContext->RSSetState(g_pRasterState);

	OutputDebugString(_T("Default Rasterize State set succeeded!\n"));

EXIT:
	SAFE_RELEASE(g_pRasterState);
	return hr;

}

HRESULT D3D11User::CreateRenderTargetView()
{
	HRESULT hr = E_FAIL;
	ID3D11Texture2D *pBackBuffer = NULL;

	//バックバッファはスワップチェーンの中にある
	//第１引数：アクセスするバッファインデックス。特殊なやつ以外0だけ
	//第２引数：使用するリソースインターフェイスの種類（GUID）
	//第３引数：バックバッファへのポインタが帰ってくる
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

	if (FAILED(hr))
	{
		OutputDebugString(_T("Error : BackBuffer extraction from SwapChain failed\n"));
		goto EXIT;
	}
	
	//レンダーターゲットビュー作成
	//第１引数：ID3D11ResourceはID3D11Texture2Dなど、バッファとテクスチャの基底インターフェイスである
	//第２引数：レンダーターゲットビューの能力を構造体で指定。2DバッファではNULLでいい。
	//第３引数：レンダーターゲットビューへのポインタが帰ってくる
	hr = m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTargetView);
	
	if (FAILED(hr))
	{
		OutputDebugString(_T("Error : RederTargetView creation failed\n"));
		goto EXIT;
	}

	OutputDebugString(_T("RederTargetView creation succeeded!\n"));

	hr = S_OK;

	//終了処理
	EXIT:
	SAFE_RELEASE(pBackBuffer);

	return hr;
}

HRESULT D3D11User::CreateViewport()
{
	HRESULT hr = E_FAIL;

	//ビューポートの設定にバックバッファの幅と高さが必要
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	hr = m_SwapChain->GetDesc(&SwapChainDesc);
	if (FAILED(hr)) goto EXIT;

	//ビューポートの設定
	D3D11_VIEWPORT Viewport[1];
	Viewport[0].TopLeftX = 0;
	Viewport[0].TopLeftY = 0;
	Viewport[0].Width    = (FLOAT)SwapChainDesc.BufferDesc.Width;
	Viewport[0].Height   = (FLOAT)SwapChainDesc.BufferDesc.Height;
	Viewport[0].MinDepth = 0.0f;
	Viewport[0].MaxDepth = 1.0f;

	m_D3DDeviceContext->RSSetViewports(1, Viewport);
	OutputDebugString(_T("Set Viewport Succeeded!\n"));

	hr = S_OK;

EXIT:

	return hr;
}

HRESULT D3D11User::CreateBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, D3D11_BIND_FLAG BindFlag, UINT CPUAccessFlag)
{
	HRESULT hr = E_FAIL;

	//頂点バッファがどのようなものなのかをD3D11_BUFFER_DESC構造体に登録
	D3D11_BUFFER_DESC BufferDesc;
	::ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.ByteWidth      = size;                    //頂点の数
	if (CPUAccessFlag == 0)
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;          //頂点バッファがCPUやGPUからどうアクセスされるかのフラグ
	else
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;          //頂点バッファがCPUやGPUからどうアクセスされるかのフラグ、こっちはCPUAccessFlagがD3D11_CPU_ACCESS_WRITEの時、
	BufferDesc.BindFlags      = BindFlag;                //バッファの種類（頂点バッファ、インデックスバッファなど）を指定
	BufferDesc.CPUAccessFlags = CPUAccessFlag;           //バッファに対するCPUのアクセス権限を指定
	BufferDesc.MiscFlags      = 0;                       //特に理由がなければ０

	//実際の頂点情報も構造体に登録する必要がある
	//D3D11_SUBRESOURCE_DATA SubresourceData;
	//SubresourceData.pSysMem          = pData; //頂点配列の戦闘ポインタ
	//SubresourceData.SysMemPitch      = 0;     //テクスチャに関する値
	//SubresourceData.SysMemSlicePitch = 0;     //テクスチャに関する値

	D3D11_SUBRESOURCE_DATA* resource = NULL;
	if (pData)
	{
		resource = new D3D11_SUBRESOURCE_DATA();
		resource->pSysMem = pData;
		resource->SysMemPitch = 0;
		resource->SysMemSlicePitch = 0;
	}

	//バッファ作成
	hr = m_D3DDevice->CreateBuffer(&BufferDesc, resource, pBuffer);

	if (FAILED(hr)){
		OutputDebugString(_T("Error: Buffer Creation failed\n"));
		goto EXIT;
	}
	//OutputDebugString(_T("Buffer Creation Succeeded!\n"));

	hr = S_OK;
EXIT:

	return hr;
}

HRESULT D3D11User::CreateVertexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag)
{
	return CreateBuffer(pBuffer, pData, size, D3D11_BIND_VERTEX_BUFFER, CPUAccessFlag);
}

HRESULT D3D11User::CreateIndexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag)
{
	return CreateBuffer(pBuffer, pData, size, D3D11_BIND_INDEX_BUFFER, CPUAccessFlag);
}

HRESULT D3D11User::CreateConstantBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag)
{
	return CreateBuffer(pBuffer, pData, size, D3D11_BIND_CONSTANT_BUFFER, CPUAccessFlag);
}

//頂点シェーダーをファイルから作成
HRESULT D3D11User::CreateVertexShaderFromFile(ID3D11VertexShader** pVertexShader, TCHAR pSrcFile[], const CHAR pFunctionName[], const CHAR pProfile[], ID3D11InputLayout** g_pLayout, D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements)
//HRESULT D3D11User::CreateVertexShaderFromFile(OUT ID3D11VertexShader** pVertexShader, IN TCHAR pSrcFile[], IN CHAR pFunctionName[], IN CHAR pProfile[], OUT ID3D11InputLayout** g_pLayout, IN D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements)
{
	HRESULT hr = E_FAIL;

	//最後のフラグはデバッグ用
	//UINT Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_OPTIMIZATION_LEVEL0;
	//ID3D10Blob* pBlob = NULL;
	//ID3D10Blob* pErrorBlob = NULL;

	//Direct3D 9 モードで実行中の場合はバージョンの変更の必要がある
	//　が今回は割愛する
	//　内容としては取得したm_FeatureLevelでD3D_FEATURE_LEVEL_?_?を判定しProfileを入れ替える
	//CHAR Profile[256];
	//行列は列優先で設定、古い形式の記述は許可しない
	UINT Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;
	ID3DBlob* pBlobN = NULL;

	//ファイルをもとにエフェクトをコンパイル
	hr = D3DCompileFromFile(
		pSrcFile,       //シェーダーコードが格納されているファイル名
		nullptr,        //シェーダー内でマクロを使う場合、そのマクロ定義の配列へのポインタ
		nullptr,        //シェーダー内で#includeを使う場合、インターフェイスへのポインタ
		pFunctionName,  //シェーダーの実行が開始されるエントリポイント関数名
		pProfile,        //シェーダーモデルを指定
		Flag1,          //シェーダーに対するコンパイルオプション
		0,              //エフェクトに対するコンパイルオプション
		&pBlobN,        //コンパイル済みシェーダーへのポインタ
		nullptr         //コンパイル時のエラーへのポインタ
	);
	//hr = D3DX11CompileFromFile(
	//	pSrcFile,        //シェーダーコードが格納されているファイル名
	//	NULL,            //シェーダー内でマクロを使う場合、そのマクロ定義の配列へのポインタ
	//	NULL,            //シェーダー内で#includeを使う場合、インターフェイスへのポインタ
	//	pFunctionName,   //シェーダーの実行が開始されるエントリポイント関数名
	//	pProfile,         //シェーダーモデルを指定
	//	Flag1,           //シェーダーコンパイルフラグ
	//	0,               //エフェクトコンパイルフラグ
	//	NULL,            //スレッドポンプインターフェイスへのポインター
	//	&pBlob,          //コンパイル済みシェーダーへのポインタ
	//	&pErrorBlob,     //コンパイル時のエラーへのポインタ
	//	NULL);           //戻り値へのポインタ(NULLでいい)
	if (hr == E_FAIL)
	{
		OutputDebugString(_T("Error: Vertex Shader Compile failed\n"));
		//if (pErrorBlob)
		//{
		//	OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		//}
		return E_FAIL;
		//goto EXIT;
	}
	

	//コンパイル済みシェーダーから頂点シェーダーオブジェクトを作成
	hr = m_D3DDevice->CreateVertexShader(
		pBlobN->GetBufferPointer(), //コンパイル済みシェーダーへのポインタ
		pBlobN->GetBufferSize(),    //コンパイル済みシェーダーへのサイズ
		NULL,                      //とりあえずNULLでいい
		pVertexShader              //ID3D11VertexShaderインターフェイスへのポインタ
		);
	if (hr == E_FAIL)
	{
		OutputDebugString(_T("Error: Vertex Shader Object creation failed\n"));
		return E_FAIL;
		//goto EXIT;
	}


	//頂点レイアウトを作成
	//　　　シェーダーで使用される入力バッファを記述するための頂点（入力）レイアウトオブジェクトを作成
	//　　　プログラマが定義した頂点構造体（D3D11_INPUT_ELEMENT_DESC）とシェーダーが必要とする情報を擦り合わせなければいけない
	BYTE* vsbuffer = (LPBYTE)pBlobN->GetBufferPointer();
	size_t vsbuffersize = pBlobN->GetBufferSize();

	hr = m_D3DDevice->CreateInputLayout(
		pLayoutDesc,  //定義した頂点構造体
		NumElements,  //pLayoutDescの入力データ型の数
		vsbuffer,     //コンパイル済みシェーダーへのポインタ
		vsbuffersize, //コンパイル済みシェーダーのサイズ
		g_pLayout     //頂点レイアウトへのポインタ
		);
	if (hr == E_FAIL){
		OutputDebugString(_T("Error: Input Layout creation failed\n"));
		goto EXIT;
	}

	hr = S_OK;
	OutputDebugString(_T("Vertex Shader Creation succeeded!\n"));

	//終了処理
EXIT:
	SAFE_RELEASE(pBlobN);
	//SAFE_RELEASE( pErrorBlob );
	
	return hr;
}

//ピクセルシェーダーをファイルから作成
HRESULT D3D11User::CreatePixelShaderFromFile(ID3D11PixelShader** pPixelShader, TCHAR pSrcFile[], const CHAR pFunctionName[], const CHAR pProfile[])
//HRESULT D3D11User::CreatePixelShaderFromFile(ID3D11PixelShader** pPixelShader, TCHAR pSrcFile[], CHAR pFunctionName[], CHAR pProfile[])
{
	HRESULT hr = E_FAIL;

	//最後のフラグはデバッグ用
	//UINT Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_OPTIMIZATION_LEVEL0;
	//ID3D10Blob* pBlob = NULL;
	//ID3D10Blob* pErrorBlob = NULL;

	//Direct3D 9 モードで実行中の場合はバージョンの変更の必要がある
	//　が今回は割愛する
	//　内容としては取得したm_FeatureLevelでD3D_FEATURE_LEVEL_?_?を判定しProfileを入れ替える
	//CHAR Profile[256];
	ID3DBlob* pBlobN = nullptr;
	
	//CHAR* Profile = "vs_5_0";
	//行列は列優先で設定、古い形式の記述は許可しない
	UINT Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;
	//最適化レベルの設定は今は行わない
	// Flat1 |= D3D10_SHADER_OPTIMIZATION_LEVEL0;など


	//ファイルをもとにエフェクトをコンパイル
	hr = D3DCompileFromFile(
		pSrcFile,       //シェーダーコードが格納されているファイル名
		nullptr,        //シェーダー内でマクロを使う場合、そのマクロ定義の配列へのポインタ
		nullptr,        //シェーダー内で#includeを使う場合、インターフェイスへのポインタ
		pFunctionName,  //シェーダーの実行が開始されるエントリポイント関数名
		pProfile,        //シェーダーモデルを指定
		Flag1,          //シェーダーに対するコンパイルオプション
		0,              //エフェクトに対するコンパイルオプション
		&pBlobN,        //コンパイル済みシェーダーへのポインタ
		nullptr         //コンパイル時のエラーへのポインタ
	);
	//hr = D3DX11CompileFromFile(
	//	pSrcFile,        //シェーダーコードが格納されているファイル名
	//	NULL,            //シェーダー内でマクロを使う場合、そのマクロ定義の配列へのポインタ
	//	NULL,            //シェーダー内で#includeを使う場合、インターフェイスへのポインタ
	//	pFunctionName,   //シェーダーの実行が開始されるエントリポイント関数名
	//	pProfile,         //シェーダーモデルを指定
	//	Flag1,           //シェーダーコンパイルフラグ
	//	0,               //エフェクトコンパイルフラグ
	//	NULL,            //スレッドポンプインターフェイスへのポインター
	//	&pBlob,          //コンパイル済みシェーダーへのポインタ
	//	&pErrorBlob,     //コンパイル時のエラーへのポインタ
	//	NULL);           //戻り値へのポインタ(NULLでいい)
	if (hr == E_FAIL)
	{
		OutputDebugString(_T("Error: Pixel Shader Compile failed\n"));
		//if (pErrorBlob)
		//{
		//	OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		//}
		goto EXIT;
	}


	//コンパイル済みシェーダーからピクセルシェーダーオブジェクトを作成
	hr = m_D3DDevice->CreatePixelShader(
		pBlobN->GetBufferPointer(), //コンパイル済みシェーダーへのポインタ
		pBlobN->GetBufferSize(),    //コンパイル済みシェーダーへのサイズ
		NULL,                      //とりあえずNULLでいい
		pPixelShader              //ID3D11VertexShaderインターフェイスへのポインタ
		);
	if (hr == E_FAIL)
	{
		OutputDebugString(_T("Error: Pixel Shader Object creation failed\n"));
		goto EXIT;
	}

	hr = S_OK;
	OutputDebugString(_T("Pixel Shader Creation succeeded!\n"));


	//終了処理
EXIT:
	SAFE_RELEASE(pBlobN);
	//SAFE_RELEASE(pBlob);
	//SAFE_RELEASE(pErrorBlob);

	return hr;
}
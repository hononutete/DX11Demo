#include "stdafx.h"
#include "ShaderCompiler.h"
#include "D3D11User.h"
ShaderCompiler::ShaderCompiler()
{
	m_shaderMap.clear();
}


ShaderCompiler::~ShaderCompiler()
{
	for (auto itr = m_shaderMap.begin(); itr != m_shaderMap.end(); ++itr) {
		SAFE_DELETE(itr->second);
	}
	m_shaderMap.clear();
}

HRESULT ShaderCompiler::CompileShaders()
{
	//default diffuse shader
	Shader* defaultDiffuseShader = new Shader();
	CompileDefaultDiffuseShader (defaultDiffuseShader);
	m_shaderMap[*(_T("DefaultDiffse"))] = defaultDiffuseShader;

	//textured diffuse shader
	Shader* texturedDiffuseShader = new Shader();
	CompileTextureDiffuseShader(texturedDiffuseShader);
	m_shaderMap[*(_T("TexturedDiffuse"))] = texturedDiffuseShader;

	return S_OK;
}


HRESULT ShaderCompiler::CompileDefaultDiffuseShader(Shader* p_Shader) 
{
	HRESULT hr = E_FAIL;
	extern D3D11User* g_pD3D11User;

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
		&(p_Shader->m_pVertexShader),  //頂点シェーダーオブジェクトへのポインタ
		_T("DefaultDiffuse.hlsl"),       //シェーダーコードが格納されているファイル名（プロジェクトルートからのパスで指定）
		"VS_Main",         //シェーダーの実行が開始されるエントリポイント関数名
		"vs_4_0",          //シェーダーモデルを指定
		&(p_Shader->m_pLayout),        //頂点レイアウトへのポインタ
		layout,            //頂点レイアウトを作るための頂点情報(D3D11_INPUT_ELEMENT_DESC)
		_countof(layout)   //頂点情報配列数
	);
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : Create vertex shader from file failed\n"));
		goto EXIT;
	}
	// ******** ピクセルシェーダーを作成 *********** //

	hr = g_pD3D11User->CreatePixelShaderFromFile(
		&(p_Shader->m_pPixelShader),   //ピクセルシェーダーオブジェクトへのポインタ
		_T("DefaultDiffuse.hlsl"),       //シェーダーコードが格納されているファイル名（プロジェクトルートからのパスで指定）
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
	hr = g_pD3D11User->CreateConstantBuffer(&(p_Shader->m_pConstantBuffersLambert), NULL, sizeof(Shader::CB_LAMBERT), 0);
	if (FAILED(hr)) {
		goto EXIT;
	}

EXIT:

	return hr;
}

HRESULT ShaderCompiler::CompileTextureDiffuseShader(Shader* p_Shader)
{
	HRESULT hr = E_FAIL;
	extern D3D11User* g_pD3D11User;
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
		&(p_Shader->m_pVertexShader),  //頂点シェーダーオブジェクトへのポインタ
		_T("TexturedDiffuse.hlsl"),       //シェーダーコードが格納されているファイル名（プロジェクトルートからのパスで指定）
		"VS_Main",         //シェーダーの実行が開始されるエントリポイント関数名
		"vs_4_0",          //シェーダーモデルを指定
		&(p_Shader->m_pLayout),        //頂点レイアウトへのポインタ
		layout,            //頂点レイアウトを作るための頂点情報(D3D11_INPUT_ELEMENT_DESC)
		_countof(layout)   //頂点情報配列数
	);
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : Create vertex shader from file failed\n"));
		goto EXIT;
	}
	// ******** ピクセルシェーダーを作成 *********** //

	hr = g_pD3D11User->CreatePixelShaderFromFile(
		&(p_Shader->m_pPixelShader),   //ピクセルシェーダーオブジェクトへのポインタ
		_T("TexturedDiffuse.hlsl"),       //シェーダーコードが格納されているファイル名（プロジェクトルートからのパスで指定）
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
	hr = g_pD3D11User->CreateConstantBuffer(&(p_Shader->m_pConstantBuffersLambert), NULL, sizeof(Shader::CB_LAMBERT), 0);
	if (FAILED(hr)) {
		goto EXIT;
	}

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
	hr = g_pD3D11User->m_D3DDevice->CreateSamplerState(&samplerDesc, &(p_Shader->m_pSamplerState));
	if (FAILED(hr)) {
		OutputDebugString(_T("error : Sampler state create failed .... \n"));
		goto EXIT;
	}
	hr = S_OK;

EXIT:

	return hr;
}
//
//HRESULT ShaderCompiler::CompileShaders() 
//{
//	HRESULT hr = E_FAIL;
//	extern D3D11User* g_pD3D11User;
//	// ******** 頂点シェーダーを作成 *********** //
//
//	//頂点要素の記述（頂点構造体側セマンティクス）
//	//　　　頂点構造体に宣言されている項目の種類、サイズを記述するD3D11_INPUT_ELEMENT_DESC構造体
//	D3D11_INPUT_ELEMENT_DESC layout[] = {
//
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
//	};
//
//	//自作DX11クラスで頂点シェーダー作成
//	hr = g_pD3D11User->CreateVertexShaderFromFile(
//		&m_pVertexShader,  //頂点シェーダーオブジェクトへのポインタ
//		_T("BasicHLSLFbx.hlsl"),       //シェーダーコードが格納されているファイル名（プロジェクトルートからのパスで指定）
//		"VS_Main",         //シェーダーの実行が開始されるエントリポイント関数名
//		"vs_4_0",          //シェーダーモデルを指定
//		&m_pLayout,        //頂点レイアウトへのポインタ
//		layout,            //頂点レイアウトを作るための頂点情報(D3D11_INPUT_ELEMENT_DESC)
//		_countof(layout)   //頂点情報配列数
//	);
//	if (FAILED(hr)) {
//		OutputDebugString(_T("ERROR : Create vertex shader from file failed\n"));
//		goto EXIT;
//	}
//	// ******** ピクセルシェーダーを作成 *********** //
//
//	hr = g_pD3D11User->CreatePixelShaderFromFile(
//		&m_pPixelShader,   //ピクセルシェーダーオブジェクトへのポインタ
//		_T("BasicHLSLFbx.hlsl"),       //シェーダーコードが格納されているファイル名（プロジェクトルートからのパスで指定）
//		"PS_Main",         //シェーダーの実行が開始されるエントリポイント関数名
//		"ps_4_0"           //シェーダーモデルを指定
//	);
//	if (FAILED(hr)) {
//		OutputDebugString(_T("ERROR : Create pixel shader from file failed\n"));
//		goto EXIT;
//	}
//
//	//**** 定数バッファを作成 **************************
//
//	//ランバート反射用定数バッファ構造体の定数バッファを作成
//	//CPUAccessFlagの設定は
//	hr = g_pD3D11User->CreateConstantBuffer(&m_pConstantBuffersLambert, NULL, sizeof(CB_LAMBERT), 0);
//	if (FAILED(hr)) {
//		goto EXIT;
//	}
//
//	// ****** サンプラーステートの設定 *****************
//	D3D11_SAMPLER_DESC samplerDesc;
//	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;         //サンプリング時に使用するフィルターを指定。これは異方性フィルター
//	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;     //0－１範囲外にあるuテクスチャー座標描画法。これはラッピング設定。タイルなどもある
//	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;     //0－１範囲外にあるvテクスチャー座標描画法。これはラッピング設定
//	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;     //0－１範囲外にあるwテクスチャー座標描画法。これはラッピング設定
//	samplerDesc.MipLODBias = 0;                            //計算されたミップマップレベルからのバイアス
//	samplerDesc.MaxAnisotropy = 16;                        //サンプリングに異方性フィルタリングを使用している場合の限界値。1-16で指定
//	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  //比較オプション
//														   //::CopyMemory(samplerDesc.BorderColor, & Utility::Co D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f), sizeof(D3DXVECTOR4)); //境界色
//	samplerDesc.MinLOD = 0;                                //アクセス可能なミップマップ下限値
//	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                //アクセス可能なミップマップ上限値
//														   //サンプラーステートの作成
//	hr = g_pD3D11User->m_D3DDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState);
//	if (FAILED(hr)) {
//		OutputDebugString(_T("error : Sampler state create failed .... \n"));
//		goto EXIT;
//	}
//	hr = S_OK;
//
//EXIT:
//	
//	return hr;
//}
//

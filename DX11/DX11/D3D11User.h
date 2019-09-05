#pragma once
//
//#ifndef DXUser_H
//#define DXUser_H

#include "UCommon.h"
//#include <windows.h>
#include <d3d11.h>
//#include <DXGI.h>

class D3D11User
{
public:
	ID3D11Device* m_D3DDevice;                   //デバイス（描画を担当する）
	IDXGIAdapter* m_Adapter;                    //アダプター
	ID3D11DeviceContext* m_D3DDeviceContext;     //デバイスコンテキスト（設定値などが格納されている）
	IDXGISwapChain* m_SwapChain;                 //バッファを管理する（バックバッファはスワップチェーンの中にある）
	ID3D11RenderTargetView*  m_RenderTargetView; //リソース（スワップチェーンのバックバッファ）を描画パイプライン（デバイス）に結合するもの
	ID3D11DepthStencilView* m_DepthStencilView;  //深度ステンシルビュー
	//HWND m_hWnd;//ウィンドウ
	D3D_FEATURE_LEVEL m_FeatureLevel;            //デバイス作成時に実際に採用されたフィーチャーレベル

	D3D11User();
	~D3D11User();

	//初期化
	HRESULT InitD3D11(HINSTANCE hInstance, HWND hWnd);

	//初期化
	HRESULT InitD3D11(WNDPROC WinProc, HINSTANCE hInstance, TCHAR* AppName);

	//終了処理
	void Invalidate();

	//頂点バッファの作成
	HRESULT CreateVertexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag);

	//インデックスバッファの作成
	HRESULT CreateIndexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag);

	//頂点シェーダーをファイルから作成
	HRESULT CreateVertexShaderFromFile(ID3D11VertexShader** pVertexShader, TCHAR pSrcFile[], const CHAR pFunctionName[], const CHAR pProfile[], ID3D11InputLayout** g_pLayout, D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements);
	//HRESULT CreateVertexShaderFromFile(OUT ID3D11VertexShader** pVertexShader,
	//	IN TCHAR pSrcFile[], IN CHAR pFunctionName[], IN CHAR pProfile[],
	//	OUT ID3D11InputLayout** g_pLayout,
	//	IN D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements);

	//ピクセルシェーダーをファイルから作成
	HRESULT CreatePixelShaderFromFile(ID3D11PixelShader** pPixelShader, TCHAR pSrcFile[], const CHAR pFunctionName[], const CHAR pProfile[]);

	//定数バッファの作成
	HRESULT CreateConstantBuffer( ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag);


private:

	//ウィンドウの作成
	HRESULT InitWindow(WNDPROC WinProc, HINSTANCE hInstance, TCHAR* AppName);

	//アダプター作成
	HRESULT CreateAdapter();

	//デバイスの作成
	HRESULT CreateDevice();

	//スワップチェーンの作成
	HRESULT CreateSwapChain();

	//スワップチェーンの作成
	HRESULT CreateSwapChain(HWND hWnd);

	//深度ステンシルビューの作成
	HRESULT CreateDepthStencilView();

	//デフォルトのラスタライザを設定
	HRESULT SetDefaultRasterize();

	//デフォルトの深度ステンシルステートを設定
	HRESULT SetDefaultDepthStencilState();

	//レンダーターゲットビューの作成
	HRESULT CreateRenderTargetView();

	//ビューポートの作成
	HRESULT CreateViewport();

	//バッファの作成
	HRESULT CreateBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, D3D11_BIND_FLAG BindFlag, UINT CPUAccessFlag);

};

//#endif
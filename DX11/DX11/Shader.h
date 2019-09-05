#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class Shader
{
public:
	__declspec(align(16))
	struct CB_LAMBERT {
		XMMATRIX matWVP;     //WVP変換行列
		XMMATRIX matNormal;  //法線変換行列
		XMFLOAT4 vecLight; //平行射影ライト
		float ambient;        //環境光ライト
	};

	//定数バッファ
	ID3D11Buffer* m_pConstantBuffersLambert = NULL;

	//頂点レイアウト（入力レイアウト）
	//DX11ではシェーダーの入力セマンティクスを自由に決めれられるようになったため
	//頂点バッファとシェーダーを結びつけるために頂点レイアウトをデバイスに登録する
	ID3D11InputLayout* m_pLayout = NULL;

	//頂点シェーダー
	ID3D11VertexShader* m_pVertexShader = NULL;

	//ピクセルシェーダー
	ID3D11PixelShader* m_pPixelShader = NULL;

	//サンプラーステート
	ID3D11SamplerState *m_pSamplerState = NULL;


	Shader();
	~Shader();
};


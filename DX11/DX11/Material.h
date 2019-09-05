#pragma once
#include <d3d11.h>
class Material
{
public:

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

	Material();
	Material(TCHAR* shaderName);

	~Material();
};


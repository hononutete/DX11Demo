#pragma once

#ifndef FBXSDK_SHARED
#define FBXSDK_SHARED
#endif 


#include "UCommon.h"
#include <vector>;
#include <unordered_map>;
#include <d3d11.h>;
#include "D3D11User.h";
#include <string.h>;
#include <stdio.h> ; 
#include <stdlib.h>;  
#include <fbxsdk.h>;
#include <DirectXMath.h>
using namespace DirectX;

class Mesh
{
public:

	//頂点構造体宣言
	typedef struct t_Vertex{
		XMFLOAT4 Vertex;
		XMFLOAT3 Normal;
		XMFLOAT4 Color;
		XMFLOAT2 UV;
	} GVERTEX;


	//テクスチャ構造体宣言
	typedef struct t_Texture{
		TCHAR MaterialName[512];
		TCHAR TexturePath[512];
		ID3D11ShaderResourceView* Texture;
	} TEXTURE;

	//ボーン構造体
	typedef struct t_Bone{
		XMMATRIX initMatrix;
		int indexCount;
		int* indices;
	};

	GVERTEX* Vertexes;  //頂点座標配列
	UINT VertexCount;  //頂点数
	ID3D11Buffer* VertexBuffer; //頂点バッファ

	UINT* Indexes;  //頂点インデックス配列
	UINT IndexCount;//頂点インデックス数
	ID3D11Buffer* IndexBuffer; //インデックスバッファ

	std::vector<TEXTURE> Textures; //メッシュで使うテクスチャ配列

	//面法線を使用する場合、もとのコントロールポイントと、分割後にどのコントロールポイントに分割されたか？の参照を保持するハッシュテーブル
	std::unordered_map<int, std::vector<int>> controlPointRef;


	//*** アニメーション関連 ***

	//アニメーション構造は将来的に分離する
	FbxMesh* m_pFbxMesh;
	FbxScene* m_pFbxScene;


	FbxTime m_Time;
	bool isAnimating;
	//アニメーション経過時間（秒）
	double m_AnimationTimeSecond;
	//ループさせるかどうか
	bool isLoop;
	//アニメーション開始時間
	double m_AnimStart;
	//アニメーション終了時間
	double m_AnimStop;
	//アニメーションのスピード
	float m_animationSpeed = 0.1f;

	//コンストラクタ
	Mesh();

	//デストラクタ
	~Mesh();

	void SetAnimation(int animStackIndex);
	
	//アニメーション時間を更新
	void UpdateAnimationTime();

	void ApplyMeshAtTime();

	bool HasAnimation();

	//*** 渡す側 ***
	//ID3D11ShaderResourceView* pSRV = NULL;　//ポインタ変数を作成	
	//g_pBoxMesh->MeshUser[DrawMeshIndex].GetTexture(_T("DiffuseColor"), &pSRV);
	//g_pD3D11User->m_D3DDeviceContext->PSSetShaderResources(0, 1, &pSRV);
	//pSRV →　アドレスが入っている（64fdf0など）
	//*pSRV　→　アドレスが指す中身
	//　ここではポインタのアドレスを引数に渡す、受け取る方ではポインタのポインタとしてうけとる
	//マテリアルの名前でテクスチャを取得
	void GetTexture(const TCHAR* pMaterialName, ID3D11ShaderResourceView** ppSRV)
	{
		TCHAR debugStr[512];
		//まず*pSRVでポインタ変数の宣言をした。そして&pSRVを引数に渡す
		//&pSRVは、ポインタ変数のアドレスを渡している。
		//
		//　ここで自動的に変換が入る。具体的にはこんな感じの変換
		//  ID3D11ShaderResourceView** ppSRV = &pSRV
		//
		//まず受け取る引数でポインタのポインタ変数**ppSRVを宣言、すでにポインタ変数pSRVのアドレスが格納されている
		//ppSRV →　pSRVポインタ変数のアドレス
		//*ppSRV　→　pSRVポインタ変数の中身（もともとpSRVは
		//*ppSRVのように*をつけると内容を参照する。つまり*ppSRVとすると
		(*ppSRV) = NULL; //ポインタのポインタのアドレスが参照する中身をクリア（本来であれば中身にはポインタ変数のアドレスが入る）
		                 //ポインタ変数をクリア
		for (int i = 0; i < (int)Textures.size(); i++)
		{
			//_stprintf_s(debugStr, 512, _T("   Material Name [%s]\n"), Textures[i].MaterialName);
			//OutputDebugString(debugStr);

			//文字列を比較する、同じだった場合0がかえる
			if (_tcscmp(Textures[i].MaterialName, pMaterialName) == 0)
			{
				(*ppSRV) = Textures[i].Texture;
				break;
			}
		}
	}
};

class MeshGroup
{
public:
	Mesh* Meshes;        //全メッシュの配列
	UINT  MeshCount;     //全メッシュ数

	//コンストラクタ
	MeshGroup(){
		Meshes = NULL;
		MeshCount = 0;
	}

	//デストラクタ
	~MeshGroup(){
		SAFE_DELETE_ARRAY(Meshes);
	}
};


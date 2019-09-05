#include "stdafx.h"
#include "PlaneMeshFactory.h"


PlaneMeshFactory::PlaneMeshFactory()
{
}


PlaneMeshFactory::~PlaneMeshFactory()
{
}

HRESULT PlaneMeshFactory::CreateMesh(MeshGroup** ppMeshGroup)
{
	HRESULT hr = E_FAIL;
	extern D3D11User* g_pD3D11User;

	//ポインタのポインタの中身にアクセスして、メッシュのインスタンスを割り当てる
	//(*ppMeshGroup)がさすものはポインタのポインタの中身
	//new演算子によって返される値は、インスタンスへのポインタ（つまりインスタンスのアドレスを格納した変数）
	//ここで行うのはインスタンスへのポインタを入れ替える作業
	(*ppMeshGroup) = new MeshGroup();

	//メッシュはひとつだけ
	(*ppMeshGroup)->MeshCount = 1;
	(*ppMeshGroup)->Meshes = new Mesh[1];

	//メッシュデータを作成
	Mesh* pMesh = new Mesh();

	//頂点
	pMesh->VertexCount = 4;
	Mesh::GVERTEX vtx[] = {
		//vertex, normal, color, uv
		{ XMFLOAT4(-1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT4(-1.0f, 0.0f,-1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT4( 1.0f, 0.0f,-1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) }
	};
	//pMesh->Vertexes = new Mesh::GVERTEX[4];
	pMesh->Vertexes = vtx;
	hr = g_pD3D11User->CreateVertexBuffer(&(pMesh->VertexBuffer), vtx, sizeof(Mesh::GVERTEX) * pMesh->VertexCount, 0);
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : Vertex Buffer creation failed for primitive plane mesh\n"));
		return hr;
	}

	//頂点インデックス
	pMesh->IndexCount = 6;
	UINT indexes[] = {
		2,1,0,3,1,2
	};
	pMesh->Indexes = indexes;
	hr = g_pD3D11User->CreateIndexBuffer(&(pMesh->IndexBuffer), indexes, pMesh->IndexCount * sizeof(UINT), 0);
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : Index Buffer creation failed primitive plane mesh\n"));
		return hr;
	}

	//メッシュをセット
	(*ppMeshGroup)->Meshes[0] = *pMesh;

	hr = S_OK;
	return hr;
}
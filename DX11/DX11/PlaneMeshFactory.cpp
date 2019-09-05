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

	//�|�C���^�̃|�C���^�̒��g�ɃA�N�Z�X���āA���b�V���̃C���X�^���X�����蓖�Ă�
	//(*ppMeshGroup)���������̂̓|�C���^�̃|�C���^�̒��g
	//new���Z�q�ɂ���ĕԂ����l�́A�C���X�^���X�ւ̃|�C���^�i�܂�C���X�^���X�̃A�h���X���i�[�����ϐ��j
	//�����ōs���̂̓C���X�^���X�ւ̃|�C���^�����ւ�����
	(*ppMeshGroup) = new MeshGroup();

	//���b�V���͂ЂƂ���
	(*ppMeshGroup)->MeshCount = 1;
	(*ppMeshGroup)->Meshes = new Mesh[1];

	//���b�V���f�[�^���쐬
	Mesh* pMesh = new Mesh();

	//���_
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

	//���_�C���f�b�N�X
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

	//���b�V�����Z�b�g
	(*ppMeshGroup)->Meshes[0] = *pMesh;

	hr = S_OK;
	return hr;
}
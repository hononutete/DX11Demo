#pragma once
#ifndef FBXSDK_SHARED
#define FBXSDK_SHARED
#endif 
#include "UCommon.h"
#include <d3d11.h>
#include <fbxsdk.h>;
#include "Mesh.h";

class FBXSDKMeshLoader
{
public:

	enum EShadingType{
		eFlatShading,
		eGouraudShading,

		//primitive
		ePrimitiveBox,
		ePrimitiveTriangle,
		ePrimitiveQuad
	};

	//�����_�����O�^�C�v
	EShadingType eShadingType;

	FBXSDKMeshLoader();
	~FBXSDKMeshLoader();

	//������
	HRESULT Initialize(ID3D11Device *pD3DDevice);

	//�I������
	void Invalidate();

	//FBX�t�@�C���̓ǂݍ���
	HRESULT LoadMeshData(TCHAR* pFileName, MeshGroup** ppMeshGroup);

private:

	FbxManager* m_pFbxManager;
	FbxScene* m_pFbxScene;

private:

	//FbxScene���̃��b�V����DirectX�Ŏg���郁�b�V�����ɕϊ�����֐�
	//���[�g�m�[�h�ɑ΂��čċA�֐���K�p���S�m�[�h�𑖍�
	HRESULT ConvertToMeshGroup(MeshGroup* pMeshGroup);

	//�m�[�h�̎q�m�[�h������ċA�֐��A���̊֐��̒��ł��ׂẴm�[�h�ɑ΂��ă��b�V���ϊ�
	void RecursiveNode(FbxNode* pNode, MeshGroup* pMeshGroup, int* pMeshIndex);

	//��̃m�[�h���̃��b�V���������b�V���N���X�ɕϊ�
	HRESULT ConvertToMesh(FbxNode* pNode, MeshGroup* pMeshGroup, int* pMeshIndex);



	//���b�V���ϊ��i�����j
	//HRESULT SetVertexBuffer(FbxMesh* pFbxMesh, Mesh* pMesh);

	HRESULT SetVertexBufferTriangleStrip(FbxMesh* pFbxMesh, Mesh* pMesh);

	//triangle list methods
	HRESULT SetVertexBufferTriangleList(FbxMesh* pFbxMesh, Mesh* pMesh);

	void SetVertexTriangleList(FbxMesh* pFbxMesh, Mesh* pMesh, int polygonCount);

	void SetNormalTriangleList(FbxMesh* pFbxMesh, Mesh* pMesh, int polygonCount, int layerCount, UINT quadIndexArray[6]);

	void SetUVTriangleList(FbxMesh* pFbxMesh, Mesh* pMesh, int polygonCount, UINT quadIndexArray[6]);

	void SetMaterialTriangleList(FbxMesh* pFbxMesh, Mesh* pMesh);

	void SetBoneTriangleList(FbxMesh* pFbxMesh, Mesh* pMesh);

	//textures
	HRESULT GetBaseTextureMap(FbxSurfaceMaterial* pMaterial, Mesh* pMesh);

	HRESULT GetTextureMap(FbxSurfaceMaterial* pMaterial, Mesh* pMesh, const char* pMaterialName);

	//primitive
	/*
	HRESULT SetVertexBufferDebugBox(FbxMesh* pFbxMesh, Mesh* pMesh);

	HRESULT SetVertexBufferDebugTriangle(FbxMesh* pFbxMesh, Mesh* pMesh);

	HRESULT SetVertexBufferDebugQuad(FbxMesh* pFbxMesh, Mesh* pMesh);
	*/

	//Utility 

	void OutputDebugInfo(Mesh* pMesh, int vertexCount, int polygonCount, int indexCount);

	UINT GetIndexCount(FbxMesh* pFbxMesh, int polygonCount);

	UINT GetTrianglePolygonCount(FbxMesh* pFbxMesh);

};
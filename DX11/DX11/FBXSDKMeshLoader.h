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

	//レンダリングタイプ
	EShadingType eShadingType;

	FBXSDKMeshLoader();
	~FBXSDKMeshLoader();

	//初期化
	HRESULT Initialize(ID3D11Device *pD3DDevice);

	//終了処理
	void Invalidate();

	//FBXファイルの読み込み
	HRESULT LoadMeshData(TCHAR* pFileName, MeshGroup** ppMeshGroup);

private:

	FbxManager* m_pFbxManager;
	FbxScene* m_pFbxScene;

private:

	//FbxScene内のメッシュをDirectXで使えるメッシュ情報に変換する関数
	//ルートノードに対して再帰関数を適用し全ノードを走査
	HRESULT ConvertToMeshGroup(MeshGroup* pMeshGroup);

	//ノードの子ノードを巡る再帰関数、この関数の中ですべてのノードに対してメッシュ変換
	void RecursiveNode(FbxNode* pNode, MeshGroup* pMeshGroup, int* pMeshIndex);

	//一つのノード内のメッシュ情報をメッシュクラスに変換
	HRESULT ConvertToMesh(FbxNode* pNode, MeshGroup* pMeshGroup, int* pMeshIndex);



	//メッシュ変換（旧式）
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
//#include "DDSTextureLoader\DDSTextureLoader.h"
#include "WICTextureLoader\WICTextureLoader.h"
#include "FBXSDKMeshLoader.h"
#include "Utility.h"
#include "D3D11User.h"
FBXSDKMeshLoader::FBXSDKMeshLoader()
{
	m_pFbxManager = NULL;
	//フラットシェーディング
	eShadingType = EShadingType::eFlatShading;
	//グーローシェーディング
	//eShadingType = EShadingType::eGouraudShading;
	//デバッグ立方体
	//eShadingType = EShadingType::ePrimitiveBox;
	//デバッグ三角
	//eShadingType = EShadingType::ePrimitiveTriangle;
	//デバッグクワッド
	//eShadingType = EShadingType::ePrimitiveQuad;
}

FBXSDKMeshLoader::~FBXSDKMeshLoader()
{
	Invalidate();
}

//初期化
HRESULT FBXSDKMeshLoader::Initialize(ID3D11Device *pD3DDevice)
{
	m_pFbxManager = FbxManager::Create();

	if (m_pFbxManager == NULL){
		OutputDebugString(_T("error: FbxManager create failed...\n"));
		return E_FAIL;
	}

	//各種設定
	//今はやらない

	//シーンオブジェクトの作成
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
	if (m_pFbxScene == NULL){
		OutputDebugString(_T("error: FbxScene create failed...\n"));
		return E_FAIL;
	}
	OutputDebugString(_T("FBXSDKMeshLoader initialize succeeded!\n"));
	return S_OK;
}

//終了処理
void FBXSDKMeshLoader::Invalidate(){
	if (m_pFbxManager)
		m_pFbxManager->Destroy();
	m_pFbxManager = NULL;
}

//FBXファイルの読み込み
HRESULT FBXSDKMeshLoader::LoadMeshData(TCHAR* pFileName, MeshGroup** ppMeshGroup){
	TCHAR debugStr[512];

	//インポーター作成
	FbxImporter* pFBXImporter = FbxImporter::Create(m_pFbxManager, "");

	//FBXのファイルパス
	char filePath[256];
	sprintf_s(filePath, "%S", pFileName);

	//インポーターの初期化
	if (FAILED(pFBXImporter->Initialize(filePath)))
	{
		OutputDebugString(_T("error: FBXImporter initialized failed\n"));
		return E_FAIL;
	}


	//FBXSDKのバージョン取得
	int sdkMajor, sdkMinor, sdkRevison;
	FbxManager::GetFileFormatVersion(sdkMajor, sdkMinor, sdkRevison);

	//FBXファイルのバージョンを取得
	int fileMajor, fileMinor, fileRevison;
	pFBXImporter->GetFileVersion(fileMajor, fileMinor, fileRevison);

	//FBXSDKとFBXファイルのバージョン比較
	if (sdkMajor != fileMajor || sdkMinor != fileMinor || sdkRevison != fileRevison){
		_stprintf_s(debugStr, 512, _T("error: version missmatch. SDK[%d, %d, %d] File[%d, %d, %d]\n"), sdkMajor, sdkMinor, sdkRevison, fileMajor, fileMinor, fileRevison);
		OutputDebugString(debugStr);
		//TODO: Support file that differs
		//return E_FAIL;
	}

	//シーンオブジェクトにインポート
	if (FAILED(pFBXImporter->Import(m_pFbxScene)))
	{
		OutputDebugString(_T("error: FBXdata import to Scene object failed\n"));
		return E_FAIL;
	}

	(*ppMeshGroup) = new MeshGroup();

	//シーンオブジェクトにインポートされたメッシュ情報をDirectXで使えるメッシュ情報に変換
	if (FAILED(ConvertToMeshGroup(*ppMeshGroup)))
	{
		OutputDebugString(_T("error: Converting FBXScene data to MeshGroup Failed\n"));
		return E_FAIL;
	}

	//インポーターはもう不要なので破棄
	pFBXImporter->Destroy();
	pFBXImporter = NULL;

	_stprintf_s(debugStr, 512, _T("loading mesh data [%s] succeeded!\n"), pFileName);
	OutputDebugString(debugStr);

	return S_OK;

}

HRESULT FBXSDKMeshLoader::ConvertToMeshGroup(MeshGroup* pMeshGroup)
{
	FbxNode *node = m_pFbxScene->GetRootNode();
	if (!node)
	{
		OutputDebugString(_T("error: Root Node is missing\n"));
		return E_FAIL;
	}

	int childCount = node->GetChildCount();
	int meshCount = 0;
	for (int i = 0; i < childCount; i++)
	{
		FbxNode* pChild = node->GetChild(i);
		FbxNodeAttribute* nodeAttribute = pChild->GetNodeAttribute();
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh){
			meshCount++;
		}
	}

	pMeshGroup->Meshes = new Mesh[meshCount];
	pMeshGroup->MeshCount = meshCount;
	int meshIndex = 0;
	int* pMeshIndex = &meshIndex;

	//run through all nodes
	RecursiveNode(node, pMeshGroup, pMeshIndex);

	return S_OK;
}

void FBXSDKMeshLoader::RecursiveNode(FbxNode* pNode, MeshGroup* pMeshGroup, int* pMeshIndex)
{
	const char* objectName = pNode->GetName();
	TCHAR debugStr[512];
	_stprintf_s(debugStr, 512, _T("Node Name [%s]\n"), objectName);
	OutputDebugString(debugStr);

	//what to do to each node? delegate can be used
	ConvertToMesh(pNode, pMeshGroup, pMeshIndex);

	int childCount = pNode->GetChildCount();
	for (int i = 0; i < childCount; i++)
	{
		FbxNode* pChild = pNode->GetChild(i);

		//do the same thing for child node
		RecursiveNode(pChild, pMeshGroup, pMeshIndex);
	}

}

HRESULT FBXSDKMeshLoader::ConvertToMesh(FbxNode* pNode, MeshGroup* pMeshGroup, int* pMeshIndex)
{
	//ノード属性を取得
	FbxNodeAttribute* nodeAttribute = pNode->GetNodeAttribute();
	if (!nodeAttribute)
	{
		OutputDebugString(_T("error: Failed to get Node Attribute\n"));
		return E_FAIL;
	}

	switch (nodeAttribute->GetAttributeType())
	{
		//ノード属性がメッシュ属性だった場合、メッシュクラスにダウンキャスト
	case FbxNodeAttribute::eMesh:
		FbxMesh* pFbxMesh = (FbxMesh*)nodeAttribute;
		if (eShadingType == EShadingType::eFlatShading)
		{
			SetVertexBufferTriangleList(pFbxMesh, &(pMeshGroup->Meshes[*pMeshIndex]));
		}
		else if (eShadingType == EShadingType::eGouraudShading)
		{
			//SetVertexBuffer(pFbxMesh, &(pMeshGroup->Meshes[*pMeshIndex]));
		}
		else if (eShadingType == EShadingType::ePrimitiveBox)
		{
			//SetVertexBufferDebugBox(pFbxMesh, &(pMeshGroup->Meshes[*pMeshIndex]));
		}
		else if (eShadingType == EShadingType::ePrimitiveTriangle)
		{
			//SetVertexBufferDebugTriangle(pFbxMesh, &(pMeshGroup->Meshes[*pMeshIndex]));
		}
		else if (eShadingType == EShadingType::ePrimitiveQuad)
		{
			//SetVertexBufferDebugQuad(pFbxMesh, &(pMeshGroup->Meshes[*pMeshIndex]));
		}

		*pMeshIndex += 1;
		break;
	}

	return S_OK;
}

/***
この関数では三角形ポリゴンを一つずつデータ作成する
（ポリゴンの描画モードがTRIANGLE_LISTである場合）

◆　使用するケース
TRIANGLE_LIST描画モードは、立方体などの角が鋭いジオメトリの描画で使用する
そのためには法線を面法線として定義する必要がある
➡　面法線：ポリゴンの面に対して法線が定義されている

◆　立方体の例
ポリゴンを一つずつ作るのでポリゴン数 x ３の頂点数が必要
　四角ポリゴン（三角形ポリゴン3頂点 * 2) * ６面 = 36個頂点
 が必要
 ***/
HRESULT FBXSDKMeshLoader::SetVertexBufferTriangleList(FbxMesh* pFbxMesh, Mesh* pMesh){
	TCHAR debugStr[512];
	HRESULT hr = E_FAIL;

	extern D3D11User* g_pD3D11User;

	//四角形ポリゴン分割用のインデックス
	UINT quadIndexArray[6] = { 0, 2, 1, 0, 3, 2 };

	////***************************************************************

	//頂点数
	int vertexCount = pFbxMesh->GetControlPointsCount();
	//ポリゴン数（これは四角形ポリゴンも含まれているため三角形ポリゴンに分割が必要）
	int polygonCount = pFbxMesh->GetPolygonCount();
	//頂点インデックス数
	int indexCount = pFbxMesh->GetPolygonVertexCount();
	//頂点インデックス配列
	int* indexArray = pFbxMesh->GetPolygonVertices();
	//レイヤー数
	int layerCount = pFbxMesh->GetLayerCount();

	//頂点座標と頂点インデックス
	SetVertexTriangleList(pFbxMesh, pMesh, polygonCount);

	//法線
	SetNormalTriangleList(pFbxMesh, pMesh, polygonCount, layerCount, quadIndexArray);

	//UV
	//UVの取得方法は基本的に法線と同じ
	SetUVTriangleList(pFbxMesh, pMesh, polygonCount, quadIndexArray);

	//マテリアル
	SetMaterialTriangleList(pFbxMesh, pMesh);

	//ボーン
	//SetBoneTriangleList(pFbxMesh, pMesh);


	//頂点バッファを作成
	//スキンメッシュアニメーションなどによって動的に頂点座標が変わる場合は、バッファに対して書き込めるようにバッファを設定しなければならない
	hr = g_pD3D11User->CreateVertexBuffer(&(pMesh->VertexBuffer), pMesh->Vertexes, sizeof(Mesh::GVERTEX) * pMesh->VertexCount, D3D11_CPU_ACCESS_WRITE);
	if (FAILED(hr)){
		OutputDebugString(_T("error : Vertex Buffer of FBX create failed...\n"));
		goto EXIT;
	}

	//インデックスバッファを作成
	//インデックスバッファの作成
	hr = g_pD3D11User->CreateIndexBuffer(&(pMesh->IndexBuffer), pMesh->Indexes, pMesh->IndexCount * sizeof(UINT), 0);
	if (FAILED(hr)){
		OutputDebugString(_T("error : Index Buffer of FBX create failed...\n"));
		goto EXIT;
	}

	//メッシュに後に使うためにFbxSceneとFbxMeshのキャッシュをセット
	pMesh->m_pFbxMesh = pFbxMesh;
	pMesh->m_pFbxScene = m_pFbxScene;

	//DEUBG OUTPUT
	OutputDebugInfo(pMesh, vertexCount, polygonCount, indexCount);

EXIT:

	return hr;
}

/***
ポリゴンの描画モードをTRIANGLE_STRIPであると想定して頂点バッファを作成する


もしこのモードで法線が定義されているのにグーローシェーディングのために頂点法線を使いたい場合は頂点の平均化の処理をしなければならない
それには以下の手順が必要

◆　グーローシェーディングで滑らかにライティングをする場合　➡　頂点法線を使用　
　　異なるポリゴンで頂点を共有しているから、頂点の重複があるということ
  　　①ひとつの頂点が複数の法線を持っていることになるので、ひとつの頂点がひとつの法線を持つように平均化が必要
	　　 ②法線はインデックスで取得したが、これはポリゴンの頂点を基準に法線が格納されている、インデックスに対応する頂点の計算が必要


	   ***/
HRESULT FBXSDKMeshLoader::SetVertexBufferTriangleStrip(FbxMesh* pFbxMesh, Mesh* pMesh){
	return E_FAIL;
}
//
//HRESULT FBXSDKMeshLoader::SetVertexBuffer(FbxMesh* pFbxMesh, Mesh* pMesh)
//{
//	TCHAR debugStr[512];
//	HRESULT hr = E_FAIL;
//
//	extern D3D11User* g_pD3D11User;
//	
//
//	//***************************************************************
//
//	//頂点数
//	int vertexCount = pFbxMesh->GetControlPointsCount();
//	//ポリゴン数
//	int polygonCount = pFbxMesh->GetPolygonCount();
//	//頂点インデックス数
//	int indexCount = pFbxMesh->GetPolygonVertexCount();
//	//頂点インデックス配列
//	int* indexArray = pFbxMesh->GetPolygonVertices();
//	//レイヤー数
//	int layerCount = pFbxMesh->GetLayerCount();
//
//
//
//	/**************************
//	頂点座標
//	***************************/
//
//	//頂点数がわかったので、Meshクラスの頂点情報配列の領域作成
//	pMesh->VertexCount = (UINT)vertexCount;
//	pMesh->Vertexes = new Mesh::GVERTEX[vertexCount];
//
//	//頂点情報をMeshクラスに実際にセット
//	for (int i = 0; i < vertexCount; i++)
//	{
//		FbxVector4 vec4 = pFbxMesh->GetControlPointAt(i); //i番目の頂点座標を取得
//
//		pMesh->Vertexes[i].Vertex.x = vec4[0];
//		pMesh->Vertexes[i].Vertex.y = vec4[1];
//		pMesh->Vertexes[i].Vertex.z = vec4[2];
//		pMesh->Vertexes[i].Vertex.w = 1.0f; //TODO: calc w
//
//		pMesh->Vertexes[i].Normal.x = 0.0f;
//		pMesh->Vertexes[i].Normal.y = 0.0f;
//		pMesh->Vertexes[i].Normal.z = 0.0f;
//
//		pMesh->Vertexes[i].Color.x = 1.0f;
//		pMesh->Vertexes[i].Color.y = 1.0f;
//		pMesh->Vertexes[i].Color.z = 1.0f;
//		pMesh->Vertexes[i].Color.w = 1.0f;
//
//		pMesh->Vertexes[i].UV.x = 0.0f;
//		pMesh->Vertexes[i].UV.y = 0.0f;
//	}
//
//	//頂点バッファの作成
//	hr = g_pD3D11User->CreateVertexBuffer(&(pMesh->VertexBuffer), pMesh->Vertexes, sizeof(pMesh->Vertexes) * pMesh->VertexCount, 0);
//	if (FAILED(hr)){
//		OutputDebugString(_T("error : Vertex Buffer of FBX create failed...\n"));
//		return E_FAIL;
//		//goto EXIT;
//	}
//
//	/**************************
//	頂点インデックス
//	***************************/
//
//
//
//	// 頂点インデックス数を求める (配列を用意する要素数として必要)
//	// fbxでは四角形ポリゴンと三角形ポリゴンがある可能性がある。そのためSDKから得られる頂点インデックスの値はあっていないから自分で求めなければならない。
//	// 四角形ポリゴンがあった場合はそれを三角形ポリゴンに分割する。
//	indexCount = 0;
//	for (int i = 0; i < polygonCount; i++)
//	{
//		//i番目のポリゴンを取得
//		int polygonVertexSize = pFbxMesh->GetPolygonSize(i);
//
//		//三角形ポリゴン
//		if (polygonVertexSize == 3)
//			indexCount += 3;
//
//		//四角形ポリゴンなら3角形ポリゴンに分割
//		else if (polygonVertexSize == 4)
//			indexCount += 6;
//	}
//	//頂点インデックス配列を作成
//	pMesh->IndexCount = indexCount;
//	pMesh->Indexes = new UINT[indexCount];
//
//	//頂点インデックスをセット
//	int currentIndex = 0;
//	UINT quadIndexArray[6] = { 0, 1, 2, 0, 2, 3 }; //四角形ポリゴン分割用のインデックス
//	for (int i = 0; i < polygonCount; i++)
//	{
//		//i番目のポリゴンを取得
//		int polygonVertexSize = pFbxMesh->GetPolygonSize(i);
//
//		//三角形ポリゴン
//		if (polygonVertexSize == 3){
//			for (int j = 0; j < 3; j++){
//				pMesh->Indexes[currentIndex] = pFbxMesh->GetPolygonVertex(i, j); //i番目のポリゴンの中のj番目の頂点は、どの頂点か？（頂点配列へのインデックス）
//				currentIndex++;
//			}
//		}
//
//		//四角形ポリゴンなら3角形ポリゴンに分割
//		else if (polygonVertexSize == 4){
//			for (int j = 0; j < 6; j++){
//				pMesh->Indexes[currentIndex] = pFbxMesh->GetPolygonVertex(i, quadIndexArray[j]); //i番目のポリゴンの中のj番目の頂点は、どの頂点か？（頂点配列へのインデックス）
//				currentIndex++;
//			}
//		}
//	}
//
//
//
//
//	//Meshクラスの頂点インデックス配列の領域作成
//	//pMesh->IndexCount = indexCount;
//	//pMesh->Indexes = new UINT[indexCount];
//
//	////頂点インデックス配列をMeshクラスにセット
//	//for (int i = 0; i < indexCount; i++)
//	//{
//	//	pMesh->Indexes[i] = indexArray[i];
//	//}
//
//	//インデックスバッファの作成
//	hr = g_pD3D11User->CreateIndexBuffer(&(pMesh->IndexBuffer), pMesh->Indexes, pMesh->IndexCount * sizeof(UINT), 0);
//	if (FAILED(hr)){
//		OutputDebugString(_T("error : Index Buffer of FBX create failed...\n"));
//		goto EXIT;
//	}
//
//	//UINT currentIndex = 0;
//	//for (int i = 0; i < polygonCount; i++) //各ポリゴンを回る
//	//{
//	//	int vertexCountInPolygon = pFbxMesh->GetPolygonSize(i); //i番目のポリゴンの頂点数
//	//	for (int j = 0; j < vertexCountInPolygon; j++)
//	//	{
//	//		pMesh->Indexes[currentIndex] = pFbxMesh->GetPolygonVertex(i, j);
//	//		currentIndex++;
//	//	}
//	//	
//	//}
//
//	/**************************
//	マテリアル
//	***************************/
//
//
//
//	/**************************
//	法線
//	***************************/
//	for (int i = 0; i < layerCount; i++){
//
//		//レイヤー
//		FbxLayer *pFbxLayer = pFbxMesh->GetLayer(i);
//
//		//レイヤーの法線情報
//		FbxLayerElementNormal *pFbxLayerElementNormal = pFbxLayer->GetNormals();
//
//		//法線はない
//		if (pFbxLayerElementNormal == 0){
//			break;
//		}
//
//		int directArrayCount = pFbxLayerElementNormal->GetDirectArray().GetCount();
//		_stprintf_s(debugStr, 512, _T("directArrayCount =>  [%d]\n"), directArrayCount);
//		OutputDebugString(debugStr);
//
//		//法線の数 and インデックスの数
//		int normalCount = pFbxLayerElementNormal->GetDirectArray().GetCount();
//		int indexCount = pFbxLayerElementNormal->GetIndexArray().GetCount();
//
//		//マッピングモード or リファレンスモード
//		FbxLayerElement::EMappingMode mappingMode = pFbxLayerElementNormal->GetMappingMode();
//		FbxLayerElement::EReferenceMode refMode = pFbxLayerElementNormal->GetReferenceMode();
//
//		_stprintf_s(debugStr, 512, _T("Noraml count [%d]  /  Index count [%d]\n"), normalCount, indexCount);
//		OutputDebugString(debugStr);
//
//		/***
//		マッピングモードがeByPolygonVertexの場合
//		➡　面法線として法線が定義されている　（ポリゴンの頂点に対して法線が定義されている）
//		　　 これは面法線として法線が定義されているということ
//		   プリミティブなどのようにポリゴンのエッジを鋭くしたい場合　➡　面法線を使用
//
//
//		   もしこのモードで法線が定義されているのにグーローシェーディングのために頂点法線を使いたい場合は頂点の平均化の処理をしなければならない
//		   それには以下の手順が必要
//
//		   ◆　グーローシェーディングで滑らかにライティングをする場合　➡　頂点法線を使用　
//		   　　異なるポリゴンで頂点を共有しているから、頂点の重複があるということ
//			 　　①ひとつの頂点が複数の法線を持っていることになるので、ひとつの頂点がひとつの法線を持つように平均化が必要
//			   　　 ②法線はインデックスで取得したが、これはポリゴンの頂点を基準に法線が格納されている、インデックスに対応する頂点の計算が必要
//				  ***/
//		if (mappingMode == FbxLayerElement::eByPolygonVertex)
//		{
//			//リファレンスモードがeDirectの場合　➡　頂点の並び順で配列に直接格納されている
//			if (refMode == FbxLayerElement::eDirect)
//			{
//				//頂点ごとのループ、これは頂点の数だけループするという意味
//				for (int u = 0; u < vertexCount; u++)
//				{
//					D3DXVECTOR3 Vec3 = D3DXVECTOR3(0, 0, 0);
//					UINT NormalIndex = 0;
//
//					//法線ベクトル重複チェック用のバッファ
//					std::vector<D3DXVECTOR3> NormalTempArray;
//					NormalTempArray.clear();
//
//
//					//ポリゴンごとのループ
//					for (int j = 0; j < polygonCount; j++)
//					{
//						//i番目のポリゴンの頂点数を取得
//						int polygonVertexCount = pFbxMesh->GetPolygonSize(j);
//
//						//i番目のポリゴン内の頂点をループ
//						for (int k = 0; k < polygonVertexCount; k++)
//						{
//							//i番目のポリゴン、j番目の頂点インデックスを取得
//							int index = pFbxMesh->GetPolygonVertex(j, k);
//
//							/*_stprintf_s(debugStr, 512, _T("  eByPolygonVertex : eDirect polygon=[%d], vertex=[%d], index=[%d]\n"), j, k, controlPointIndex);
//							OutputDebugString(debugStr);*/
//
//							//FbxVector4 fbxVec4 = pFbxLayerElementNormal->GetDirectArray().GetAt(controlPointIndex);
//							//_stprintf_s(debugStr, 512, _T("  eByPolygonVertex : eDirect polygon=[%d], vertex=[%d], index=[%d], normal(x,y,z) = ([%f],[%f],[%f])\n"), j, k, controlPointIndex, fbxVec4[0], fbxVec4[1], fbxVec4[2]);
//							//OutputDebugString(debugStr);
//
//							//一致するすべての法線を足して、それを正規化することで頂点における法線ベクトルを平均化する
//							//コントロールポイントのインデックスが一致している頂点、この法線を加算する
//							if (u == index)
//							{
//								//法線ベクトル
//								//GetDirectArray()：配列のサイズは頂点インデックスと同じ個数（6面体では24個４＊６）
//								FbxVector4 fbxVec4 = pFbxLayerElementNormal->GetDirectArray().GetAt(NormalIndex);
//
//								bool find = false;
//
//								//バッファをみて、既に加算済みかチェック、加算済みはスキップ
//								for (int w = 0; w < (int)NormalTempArray.size(); w++){
//									if (NormalTempArray[w].x == fbxVec4[0] && NormalTempArray[w].y == fbxVec4[1] && NormalTempArray[w].z == fbxVec4[2]){
//										find = true;
//										break;
//									}
//								}
//
//								//法線ベクトルが完全に一致する頂点は重複して加算しない
//								if (find == false)
//								{
//									NormalTempArray.push_back(D3DXVECTOR3((float)fbxVec4[0], (float)fbxVec4[1], (float)fbxVec4[2]));
//
//									Vec3 = D3DXVECTOR3(
//										Vec3.x + (float)fbxVec4[0],
//										Vec3.y + (float)fbxVec4[1],
//										Vec3.z + (float)fbxVec4[2]);
//								}
//
//							}
//							NormalIndex++;
//
//						}
//
//					}
//
//					//単位ベクトルの計算
//					D3DXVec3Normalize(&Vec3, &Vec3);
//
//					//法線ベクトルを加算
//					::CopyMemory(&(pMesh->Vertexes[u].Normal), &Vec3, sizeof(D3DXVECTOR3));
//				}
//
//
//				//この方法ではコントロールポイントの取得になってしまうのでここでは使えない
//				//for (int i = 0; i < normalCount; i++)
//				//{
//				//	//i番目の頂点インデックス
//				//	float x = (float)pFbxLayerElementNormal->GetDirectArray().GetAt(i)[0];
//				//	float y = (float)pFbxLayerElementNormal->GetDirectArray().GetAt(i)[1];
//				//	float z = (float)pFbxLayerElementNormal->GetDirectArray().GetAt(i)[2];
//				//	_stprintf_s(debugStr, 512, _T("  eByPolygonVertex : eDirect (x,y,z) = ([%f],[%f],[%f])\n"), x, y, z);
//				//	OutputDebugString(debugStr);
//				//}
//			}
//			//リファレンスモードがeIndexToDirectの場合　➡　法線の並び順がインデックスで示されていて、インデックスに対応した法線が配列に格納されてある
//			//これは対応しない
//
//		}
//		//マッピングモードがeByControlPointの場合　➡　コントロール点（）に対して法線が定義されている場合
//		else if (mappingMode == FbxLayerElement::eByControlPoint){
//			//リファレンスモードがeDirectの場合　➡　頂点の並び順で配列に格納されている
//			if (refMode == FbxLayerElement::eDirect){
//				float x = (float)pFbxLayerElementNormal->GetDirectArray().GetAt(i)[0];
//				float y = (float)pFbxLayerElementNormal->GetDirectArray().GetAt(i)[1];
//				float z = (float)pFbxLayerElementNormal->GetDirectArray().GetAt(i)[2];
//				_stprintf_s(debugStr, 512, _T("  eByControlPoint : eDirect (x,y,z) = ([%f],[%f],[%f])\n"), x, y, z);
//				OutputDebugString(debugStr);
//			}
//			//リファレンスモードがeIndexToDirectの場合　➡　法線の並び順がインデックスで示されていて、インデックスに対応した法線が配列に格納されてある
//			//これは対応しない
//		}
//
//
//	}
//
//	/**************************
//	UV
//	***************************/
//
//
//
//	/**************************
//	テクスチャ
//	***************************/
//
//	//DEUBG OUTPUT
//	OutputDebugInfo(pMesh, vertexCount, polygonCount, indexCount);
//
//	hr = S_OK;
//
//EXIT:
//
//	return hr;
//}

void FBXSDKMeshLoader::SetVertexTriangleList(FbxMesh* pFbxMesh, Mesh* pMesh, int polygonCount){
	TCHAR debugStr[512];

	//***************************************************************

	//頂点数
	int vertexCount = pFbxMesh->GetControlPointsCount();
	//ポリゴン数（これは四角形ポリゴンも含まれているため三角形ポリゴンに分割が必要）
	//int polygonCount = pFbxMesh->GetPolygonCount();
	//頂点インデックス数
	int indexCount = pFbxMesh->GetPolygonVertexCount();
	//頂点インデックス配列
	int* indexArray = pFbxMesh->GetPolygonVertices();
	//レイヤー数
	//int layerCount = pFbxMesh->GetLayerCount();

	for (int i = 0; i < vertexCount; i++){
		FbxVector4 vec4 = pFbxMesh->GetControlPointAt(i);

		_stprintf_s(debugStr, 512, _T("### [%d] =>  Vertex ([%f], [%f], [%f])\n"),
			i, vec4[0], vec4[1], vec4[2]);
		OutputDebugString(debugStr);

	}

	/**************************
	頂点座標と頂点インデックス
	***************************/

	//三角リスト（面法線）でジオメトリを作成する場合、作成しなければならない頂点数＝三角ポリゴン数*3
	//三角ポリゴンに分割し、その頂点数を計算
	int trianglePolygonSize = GetTrianglePolygonCount(pFbxMesh);
	int sizeOfVertexArray = trianglePolygonSize * 3;

	//四角形ポリゴン分割用のインデックス
	UINT quadIndexArray[6] = { 0, 2, 1, 0, 3, 2 };

	//必要な頂点データ数がわかったので、Meshクラスの頂点情報配列の領域作成
	pMesh->VertexCount = (UINT)sizeOfVertexArray;
	pMesh->Vertexes = new Mesh::GVERTEX[sizeOfVertexArray];

	//頂点インデックス数も上の必要な頂点データ数と同じ個数になる（頂点の重複がないから）
	pMesh->IndexCount = sizeOfVertexArray;
	pMesh->Indexes = new UINT[sizeOfVertexArray];



	//各ポリゴン
	int index = 0; //カウント用
	for (int i = 0; i < polygonCount; i++)
	{
		//i番目のポリゴンの頂点数を取得
		int polygonVertexSize = pFbxMesh->GetPolygonSize(i);

		//ポリゴンの頂点をそれぞれ計算

		//三角形ポリゴン
		if (polygonVertexSize == 3)
		{
			for (int j = 0; j < 3; j++)
			{
				//i番目のポリゴン内のj番目の頂点が、頂点配列（コントロールポイント配列）のどのインデックスに対応しているかを取得
				int controlPointIndex = pFbxMesh->GetPolygonVertex(i, j);

				//インデックスのキーが存在すれば、その分割インデックスに追加、キーが存在しなければ新しい要素として追加
				auto itr = pMesh->controlPointRef.find(controlPointIndex);
				if (itr == pMesh->controlPointRef.end()){ //設定されている場合は最後のイテレーターを返す
					pMesh->controlPointRef[controlPointIndex].push_back(index);
				}
				else{
					pMesh->controlPointRef[controlPointIndex].push_back(index);
				}

				//i番目のポリゴン内のj番目の頂点の座標
				FbxVector4 vec4 = pFbxMesh->GetControlPointAt(controlPointIndex);
				pMesh->Vertexes[index].Vertex.x = vec4[0];
				pMesh->Vertexes[index].Vertex.y = vec4[1];
				pMesh->Vertexes[index].Vertex.z = vec4[2];
				pMesh->Vertexes[index].Vertex.w = 1.0f; //TODO: calc w

				//頂点カラー
				pMesh->Vertexes[index].Color.x = 1.0f;
				pMesh->Vertexes[index].Color.y = 1.0f;
				pMesh->Vertexes[index].Color.z = 1.0f;
				pMesh->Vertexes[index].Color.w = 1.0f;

				//ついでに頂点インデックスも対応させる（もちろん今の頂点配列内のインデックスを設定）
				pMesh->Indexes[index] = (UINT)index;

				//カウントアップ
				index++;
			}
		}
		//四角形ポリゴン
		else if (polygonVertexSize == 4)
		{
			//クワッドの分割インデックスを生成
			for (int k = 0; k < 4; k++){
				int controlPointIndex = pFbxMesh->GetPolygonVertex(i, k);
				_stprintf_s(debugStr, 512, _T("Polygon [%d]  /  index In Polygon [%d] => controlPointIndex [%d]\n"), i, k, controlPointIndex);
				OutputDebugString(debugStr);
			}


			//ひとつめの三角分割ポリゴン
			for (int j = 0; j < 6; j++)
			{
				//分割インデックスを取得
				int indexInPolygon = quadIndexArray[j];

				//i番目のポリゴン内のj番目の頂点が、頂点配列（コントロールポイント配列）のどのインデックスに対応しているかを取得
				int controlPointIndex = pFbxMesh->GetPolygonVertex(i, indexInPolygon);

				//インデックスのキーが存在すれば、その分割インデックスに追加、キーが存在しなければ新しい要素として追加
				auto itr = pMesh->controlPointRef.find(controlPointIndex);
				if (itr == pMesh->controlPointRef.end()){ //設定されている場合は最後のイテレーターを返す
					pMesh->controlPointRef[controlPointIndex].push_back(index);
				}
				else{
					pMesh->controlPointRef[controlPointIndex].push_back(index);
				}

				//i番目のポリゴン内のj番目の頂点の座標
				FbxVector4 vec4 = pFbxMesh->GetControlPointAt(controlPointIndex);
				pMesh->Vertexes[index].Vertex.x = vec4[0];
				pMesh->Vertexes[index].Vertex.y = vec4[1];
				pMesh->Vertexes[index].Vertex.z = vec4[2];
				pMesh->Vertexes[index].Vertex.w = 1.0f; //TODO: calc w

				//頂点カラー
				pMesh->Vertexes[index].Color.x = 1.0f;
				pMesh->Vertexes[index].Color.y = 1.0f;
				pMesh->Vertexes[index].Color.z = 1.0f;
				pMesh->Vertexes[index].Color.w = 1.0f;

				//ついでに頂点インデックスも対応させる（もちろん今の頂点配列内のインデックスを設定）
				pMesh->Indexes[index] = (UINT)index;

				//カウントアップ
				index++;
			}
		}

	}
	
}

void FBXSDKMeshLoader::SetNormalTriangleList(FbxMesh* pFbxMesh, Mesh* pMesh, int polygonCount, int layerCount, UINT quadIndexArray[6]){
	TCHAR debugStr[512];
	for (int i = 0; i < layerCount; i++)
	{

		//i番目のレイヤー
		FbxLayer *pFbxLayer = pFbxMesh->GetLayer(i);

		//レイヤーの法線情報
		FbxLayerElementNormal *pFbxLayerElementNormal = pFbxLayer->GetNormals();

		//法線はない
		if (pFbxLayerElementNormal == 0){
			break;
		}

		//法線があるので続行
		int directArrayCount = pFbxLayerElementNormal->GetDirectArray().GetCount();

		//法線の数 and インデックスの数
		int normalCount = pFbxLayerElementNormal->GetDirectArray().GetCount();
		int indexCount = pFbxLayerElementNormal->GetIndexArray().GetCount();

		_stprintf_s(debugStr, 512, _T("Noraml array size [%d]  /  Index array size [%d]\n"), normalCount, indexCount);
		OutputDebugString(debugStr);


		//マッピングモード or リファレンスモード
		FbxLayerElement::EMappingMode mappingMode = pFbxLayerElementNormal->GetMappingMode();
		FbxLayerElement::EReferenceMode refMode = pFbxLayerElementNormal->GetReferenceMode();

		//マッピングモードがeByPolygonVertexの場合　➡　面法線として法線が定義されている
		if (mappingMode == FbxLayerElement::eByPolygonVertex)
		{
			//リファレンスモードがeDirectの場合　➡　n番目の要素はGetDirectArray()関数.GetAt(n)で取得することができる。
			//eByPolygonVertexの場合、ポリゴンの頂点に対して法線が定義されているので、ポリゴンの頂点の個数だけ法線配列要素が存在し、その順番で格納されているはずである！
			//立方体の場合、四角形ポリゴン６面 * ４頂点 = 24法線
			if (refMode == FbxLayerElement::eDirect)
			{
				//各ポリゴン
				int index = 0; //カウント用
				int normalIndex = 0;
				for (int i = 0; i < polygonCount; i++)
				{
					//i番目のポリゴンの頂点数を取得
					int polygonVertexSize = pFbxMesh->GetPolygonSize(i);

					//ポリゴンの頂点をそれぞれ計算

					//三角形ポリゴン
					if (polygonVertexSize == 3)
					{
						for (int j = 0; j < 3; j++)
						{
							//i番目のポリゴン内のj番目の頂点が、頂点配列（コントロールポイント配列）のどのインデックスに対応しているかを取得
							int controlPointIndex = pFbxMesh->GetPolygonVertex(i, j);

							//法線ベクトル
							FbxVector4 fbxVec4 = pFbxLayerElementNormal->GetDirectArray().GetAt(normalIndex + j);

							//i番目のポリゴン内のj番目の頂点の座標
							pMesh->Vertexes[index].Normal.x = (float)fbxVec4[0];
							pMesh->Vertexes[index].Normal.y = (float)fbxVec4[1];
							pMesh->Vertexes[index].Normal.z = (float)fbxVec4[2];

							//カウントアップ
							index++;
						}
					}
					//四角形ポリゴン
					else if (polygonVertexSize == 4)
					{
						//クワッドの分割インデックスを生成
						for (int k = 0; k < 4; k++){
							FbxVector4 v = pFbxLayerElementNormal->GetDirectArray().GetAt(normalIndex + k);
							int controlPointIndex = pFbxMesh->GetPolygonVertex(i, k);
							//_stprintf_s(debugStr, 512, _T("___ Polygon [%d]  /  index In Polygon [%d] => normal([%f],[%f],[%f])\n"), i, k, v[0], v[1], v[2]);
							//OutputDebugString(debugStr);
						}

						//ひとつめの三角分割ポリゴン
						for (int j = 0; j < 6; j++)
						{
							//分割インデックスを取得
							int indexInPolygon = quadIndexArray[j];

							//i番目のポリゴン内のj番目の頂点が、頂点配列（コントロールポイント配列）のどのインデックスに対応しているかを取得
							int controlPointIndex = pFbxMesh->GetPolygonVertex(i, indexInPolygon);

							//法線ベクトル
							FbxVector4 fbxVec4 = pFbxLayerElementNormal->GetDirectArray().GetAt(normalIndex + indexInPolygon);

							////i番目のポリゴン内のj番目の頂点の座標
							pMesh->Vertexes[index].Normal.x = (float)fbxVec4[0];
							pMesh->Vertexes[index].Normal.y = (float)fbxVec4[1];
							pMesh->Vertexes[index].Normal.z = (float)fbxVec4[2];

							//カウントアップ
							index++;
						}
					}
					normalIndex += polygonVertexSize;
				}
			}
			//リファレンスモードがeIndexToDirectの場合　➡　法線の並び順がインデックスで示されていて、インデックスに対応した法線が配列に格納されてある
			{
				//これは対応しない
			}

		}
		//マッピングモードがeByControlPointの場合　➡　頂点法線として定義されている
		else if (mappingMode == FbxLayerElement::eByControlPoint)
		{

		}


	}
}

void FBXSDKMeshLoader::SetUVTriangleList(FbxMesh* pFbxMesh, Mesh* pMesh, int polygonCount, UINT quadIndexArray[6]){
	TCHAR debugStr[512];
	OutputDebugString(_T("\n\n### UV LOG ###\n"));

	//UV のカウント
	int uvCount = pFbxMesh->GetUVLayerCount();
	_stprintf_s(debugStr, 512, _T("   UV Count : [%d]\n"), uvCount);
	OutputDebugString(debugStr);

	if (uvCount == 0) {
		return;
	}

	//UVSet名のリストを取得
	FbxStringList lUVSetNameList;
	pFbxMesh->GetUVSetNames(lUVSetNameList);
	for (int i = 0; i < lUVSetNameList.GetCount(); i++){
		TCHAR TUVSetName[512];
		Utility::ConvertCharToTChar(TUVSetName, lUVSetNameList.GetStringAt(i));
		_stprintf_s(debugStr, 512, _T("   UVSet Name : [%s]\n"), TUVSetName);
		OutputDebugString(debugStr);
	}


	//レイヤー0からUV情報を取得
	FbxGeometryElementUV* leGUV = pFbxMesh->GetElementUV(0);
	FbxGeometryElement::EMappingMode mappingMode = leGUV->GetMappingMode();
	FbxGeometryElement::EReferenceMode refMode = leGUV->GetReferenceMode();

	//TCHAR relativeFileName[512];
	//ConvertCharToTChar(relativeFileName, leGUV->GetName());
	//_stprintf_s(debugStr, 512, _T("   UVSet Name : [%s]\n"), relativeFileName);
	//OutputDebugString(debugStr);

	//ポリゴンの頂点ごとにUVデータが紐づけられている
	if (mappingMode == FbxGeometryElement::eByPolygonVertex)
	{
		if (refMode == FbxGeometryElement::eDirect)
		{
			OutputDebugString(_T("eByPolygonVertex / eDirect\n"));
		}

		//リファレンスモードがeDirectの場合
		//UVが格納されている配列GetDirectArray()と、
		//このUV配列へのインデックスが格納されているインデックス配列GetIndexArray()の2つの配列を使っている
		else if (refMode == FbxGeometryElement::eIndexToDirect)
		{
			OutputDebugString(_T("eByPolygonVertex / eIndexToDirect\n"));

			//UVのインデックス配列を取得
			//これはなんの順番でならんでいるのか？24個あるから多分インデックス順
			FbxLayerElementArrayTemplate<int>* uvIndexArray = &leGUV->GetIndexArray();

			//UVインデックス配列の要素数
			int uvIndexCount = uvIndexArray->GetCount();
			_stprintf_s(debugStr, 512, _T("   uvIndexCount [%d]\n"), uvIndexCount);
			OutputDebugString(debugStr);

			//各ポリゴン
			int index = 0; //カウント用
			int baseIndex = 0;
			for (int i = 0; i < polygonCount; i++)
			{
				//i番目のポリゴンの頂点数を取得
				int polygonVertexSize = pFbxMesh->GetPolygonSize(i);

				//三角形ポリゴン
				if (polygonVertexSize == 3)
				{
					for (int j = 0; j < 3; j++)
					{
						//UVデータ配列へのインデックス
						int indexToArray = uvIndexArray->GetAt(baseIndex + j);

						//UVデータ配列へのインデックスを使ってUVデータを取得
						pMesh->Vertexes[index].UV.x = (float)leGUV->GetDirectArray().GetAt(indexToArray)[0];
						pMesh->Vertexes[index].UV.y = (float)leGUV->GetDirectArray().GetAt(indexToArray)[1];

						//カウントアップ
						index++;
					}
				}
				//四角形ポリゴン
				else if (polygonVertexSize == 4)
				{

					//ひとつめの三角分割ポリゴン
					for (int j = 0; j < 6; j++)
					{
						//分割インデックスを取得
						int indexInPolygon = quadIndexArray[j];

						//UVデータ配列へのインデックス
						int indexToArray = uvIndexArray->GetAt(baseIndex + indexInPolygon);

						//UVデータ配列へのインデックスを使ってUVデータを取得
						pMesh->Vertexes[index].UV.x = (float)leGUV->GetDirectArray().GetAt(indexToArray)[0];
						pMesh->Vertexes[index].UV.y = (float)leGUV->GetDirectArray().GetAt(indexToArray)[1];


						//カウントアップ
						index++;
					}
				}
				baseIndex += polygonVertexSize;
			}
		}
	}
	else if (mappingMode == FbxGeometryElement::eByControlPoint)
	{
		if (refMode == FbxGeometryElement::eDirect)
		{
			OutputDebugString(_T("eByControlPoint / eDirect\n"));
		}
		else if (refMode == FbxGeometryElement::eIndexToDirect)
		{
			OutputDebugString(_T("eByControlPoint / eIndexToDirect\n"));
		}
	}
}

void FBXSDKMeshLoader::SetMaterialTriangleList(FbxMesh* pFbxMesh, Mesh* pMesh){
	TCHAR debugStr[512];
	OutputDebugString(_T("\n\n### MATERIAL LOG ###\n"));

	//マテリアルレイヤーを取得
	//マッピングモード（ポリゴンとマテリアルの参照方法の定義方法）
	//とポリゴンへの
	FbxLayerElementMaterial *lpLayerMaterial = pFbxMesh->GetLayer(0)->GetMaterials();
	if (lpLayerMaterial)
	{
		//ひとつのメッシュにひとつだけマテリアルが設定されている場合
		if (lpLayerMaterial->GetMappingMode() == FbxLayerElementMaterial::eAllSame)
		{
			_stprintf_s(debugStr, 512, _T(" Material Mapping Mode : eAllSame\n"));
			OutputDebugString(debugStr);

			//マテリアルが格納されているインデックスを取得（eAllSameではひとつしかないとわかっているから0でいい）
			int matIndex = lpLayerMaterial->GetIndexArray()[0];

			//ノードのマテリアルを取得（ノードの属性がメッシュの時にメッシュの処理を行っているのでノードとメッシュは1:1の関係）
			//この時にさきに取得したマテリアルの参照インデックスをつかう
			FbxSurfaceMaterial* lpSurfaceMaterial = pFbxMesh->GetNode()->GetMaterial(matIndex);

			//マテリアルを解析（テクスチャの作成）
			GetBaseTextureMap(lpSurfaceMaterial, pMesh);

		}
		//ポリゴンごとにマテリアルが設定されている場合
		else if (lpLayerMaterial->GetMappingMode() == FbxLayerElementMaterial::eByPolygon)
		{
			_stprintf_s(debugStr, 512, _T(" Material Mapping Mode : eByPolygon\n"));
		}
	}
}

void FBXSDKMeshLoader::SetBoneTriangleList(FbxMesh* pFbxMesh, Mesh* pMesh){
	TCHAR debugStr[512];
	OutputDebugString(_T("\n\n### SKELTON LOG ###\n"));

	int lClusterCount = 0;
	int lSkinCount = 0;

	//スキンの数を取得（これが０の場合はボーンが入っていない）
	lSkinCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);

	_stprintf_s(debugStr, 512, _T(" Skin Count [%d]\n"), lSkinCount);
	OutputDebugString(debugStr);

	//それぞれのスキンについて
	for (int i = 0; i < lSkinCount; i++)
	{
		//i番目のスキンを取得
		FbxSkin* lFbxSkin = (FbxSkin*)pFbxMesh->GetDeformer(i, FbxDeformer::eSkin);

		//スキンに含まれるボーンの数を取得
		lClusterCount = lFbxSkin->GetClusterCount();
		_stprintf_s(debugStr, 512, _T(" Skin [%d]\n"), i);
		OutputDebugString(debugStr);
		_stprintf_s(debugStr, 512, _T(" --- Bone Count [%d]\n"), lClusterCount);
		OutputDebugString(debugStr);

		//それぞれのボーンについて
		for (int j = 0; j < lClusterCount; j++)
		{
			//j番目のボーンを取得
			FbxCluster* lCluster = lFbxSkin->GetCluster(j);

			int lIndexCount = lCluster->GetControlPointIndicesCount(); //ボーンに紐づけられている頂点数
			int* lIndices = lCluster->GetControlPointIndices();        //ボーンに紐づけられている頂点インデックス配列
			double* lWeights = lCluster->GetControlPointWeights();    //ボーンに紐づけられている頂点のウェイト

			_stprintf_s(debugStr, 512, _T("\n --- Bone [%d] vertex count [%d]\n"), j, lIndexCount);
			OutputDebugString(debugStr);

			//初期姿勢行列
			FbxAMatrix initMat;
			lCluster->GetTransformLinkMatrix(initMat);

			FbxVector4 lrowA = initMat.GetRow(0);
			FbxVector4 lrowB = initMat.GetRow(1);
			FbxVector4 lrowC = initMat.GetRow(2);
			FbxVector4 lrowD = initMat.GetRow(3);

			_stprintf_s(debugStr, 512, _T(" --- Init Matrix ( [%f], [%f], [%f], [%f] ) \n"), lrowA[0], lrowA[1], lrowA[2], lrowA[3]);
			OutputDebugString(debugStr);
			_stprintf_s(debugStr, 512, _T("                 ( [%f], [%f], [%f], [%f] ) \n"), lrowB[0], lrowB[1], lrowB[2], lrowB[3]);
			OutputDebugString(debugStr);
			_stprintf_s(debugStr, 512, _T("                 ( [%f], [%f], [%f], [%f] ) \n"), lrowC[0], lrowC[1], lrowC[2], lrowC[3]);
			OutputDebugString(debugStr);
			_stprintf_s(debugStr, 512, _T("                 ( [%f], [%f], [%f], [%f] ) \n"), lrowD[0], lrowD[1], lrowD[2], lrowD[3]);
			OutputDebugString(debugStr);

			for (int k = 0; k < lIndexCount; k++)
			{
				//頂点インデックス
				int index = lIndices[k];

				//頂点ウェイト
				double weight = lWeights[k];



				_stprintf_s(debugStr, 512, _T("     --- index [%d]\n"), index);
				OutputDebugString(debugStr);
				_stprintf_s(debugStr, 512, _T("     --- weight [%lf]\n"), weight);
				OutputDebugString(debugStr);
			}

		}

	}
}



// Texture

HRESULT FBXSDKMeshLoader::GetBaseTextureMap(FbxSurfaceMaterial* pMaterial, Mesh* pMesh)
{
	HRESULT hr = E_FAIL;
	TCHAR debugStr[512];

	//diffuse texture
	hr = GetTextureMap(pMaterial, pMesh, FbxSurfaceMaterial::sDiffuse);

	//他にテクスチャの種類をサポートする場合、追加...
	return hr;
}

HRESULT FBXSDKMeshLoader::GetTextureMap(FbxSurfaceMaterial* pMaterial, Mesh* pMesh, const char* pMaterialName){
	TCHAR debugStr[512];
	HRESULT hr = E_FAIL;
	extern D3D11User* g_pD3D11User;
	
	//テクスチャのロード方法を指定する構造体
	//D3DX11_IMAGE_LOAD_INFO info;
	//::ZeroMemory(&info, sizeof(D3DX11_IMAGE_LOAD_INFO));
	//info.Width = D3DX11_DEFAULT;
	//info.Height = D3DX11_DEFAULT;
	//info.Depth = D3DX11_DEFAULT;
	//info.FirstMipLevel = D3DX11_DEFAULT;
	//info.MipLevels = 0;
	//info.Usage = D3D11_USAGE_DEFAULT;
	//info.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//info.CpuAccessFlags = 0;
	//info.MiscFlags = 0;
	//info.Format = DXGI_FORMAT_FROM_FILE;
	//info.Filter = D3DX11_FILTER_POINT;
	//info.MipFilter = D3DX11_FILTER_POINT;
	//info.pSrcInfo = NULL;

	OutputDebugString(_T("\n\n### TEXTURE MAPPING LOG ###\n"));

	//マテリアルのプロパティを取得
	FbxProperty lProperty = pMaterial->FindProperty(pMaterialName);

	//マテリアル名を取得
	TCHAR materialName[512];
	Utility::ConvertCharToTChar(materialName, lProperty.GetName().Buffer());
	_stprintf_s(debugStr, 512, _T(" Material Name [%s]\n"), materialName);
	OutputDebugString(debugStr);

	//プロパティからブレンドテクスチャの数を取得
	int lLayeredTextureCount = lProperty.GetSrcObjectCount<FbxLayeredTexture>();
	_stprintf_s(debugStr, 512, _T(" Blend Texture Count : [%d]\n"), lLayeredTextureCount);
	OutputDebugString(debugStr);


	//普通のテクスチャ
	if (lLayeredTextureCount == 0)
	{
		OutputDebugString(_T(" Material has Simple FbxTexture...\n"));

		//マテリアルのプロパティからテクスチャの数を取得
		int lNbTextureCount = lProperty.GetSrcObjectCount<FbxTexture>();

		_stprintf_s(debugStr, 512, _T(" Material has Simple FbxTexture... count of texture is [%d]\n"), lNbTextureCount);
		OutputDebugString(debugStr);

		for (int k = 0; k < lNbTextureCount; k++)
		{
			//k番目のテクスチャ
			FbxTexture* lpTexture = lProperty.GetSrcObject<FbxTexture>(k);

			//FbxFileTextureへキャスト
			FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(lpTexture);
			if (lFileTexture)
			{
				_stprintf_s(debugStr, 512, _T("   [%d]th texture: found FbxFileTexture\n"), k);
				OutputDebugString(debugStr);

				TCHAR fileName[512];
				TCHAR relativeFileName[512];
				TCHAR UVSetName[512];

				Utility::ConvertCharToTChar(fileName, lFileTexture->GetFileName());
				Utility::ConvertCharToTChar(relativeFileName, lFileTexture->GetRelativeFileName());
				Utility::ConvertCharToTChar(UVSetName, lFileTexture->UVSet.GetName().Buffer());

				_stprintf_s(debugStr, 512, _T("   --- file name [%s]\n"), fileName);
				OutputDebugString(debugStr);
				_stprintf_s(debugStr, 512, _T("   --- relative file name [%s]\n"), relativeFileName);
				OutputDebugString(debugStr);
				_stprintf_s(debugStr, 512, _T("   --- UVSet name [%s]\n"), UVSetName);
				OutputDebugString(debugStr);

				TCHAR rcFileName[512] = _T("Resources\\");
				lstrcat(rcFileName, relativeFileName);
				_stprintf_s(debugStr, 512, _T("   --- relative file name from project [%s]\n"), rcFileName);
				OutputDebugString(debugStr);


				//メッシュに複数のテクスチャが含まれていた場合、UVがどのテクスチャと対応しているか？を判定しなければいけない
				//しかし、とりあえずはひとつのテクスチャのみをサポート

				//テクスチャ構造体を作成
				Mesh::TEXTURE tex;
				tex.MaterialName[0] = NULL;
				tex.TexturePath[0] = NULL;
				tex.Texture = NULL;

				//画像パスの設定
				_tcscpy_s(tex.TexturePath, rcFileName);

				//テクスチャー名を設定
				_tcscpy_s(tex.MaterialName, materialName);


				_stprintf_s(debugStr, 512, _T("   --- Material name [%s]\n"), tex.MaterialName);
				OutputDebugString(debugStr);

				

				//シェーダーリソースビューを作成
				ID3D11Resource* rcTexture;
				hr = CreateWICTextureFromFile(g_pD3D11User->m_D3DDevice, rcFileName, &rcTexture, &tex.Texture);
				
				if (FAILED(hr)){
					OutputDebugString(_T("error : Create Shader Resource view fialed...\n"));
					goto EXIT;
				}
				pMesh->Textures.push_back(tex);
			}
			else{
				_stprintf_s(debugStr, 512, _T("   [%d]th texture: cast to FbxFileTexture failed\n"), k);
				OutputDebugString(debugStr);
			}
		}

	}
	//ブレンドテクスチャ
	else
	{
		OutputDebugString(_T(" Material has Blend FbxTexture...\n"));
		//ブレンドテクスチャが複数ある場合それぞれについて
		for (int j = 0; j < lLayeredTextureCount; j++)
		{
			//プロパティからj番目のブレンドテクスチャを取得
			FbxLayeredTexture* lpLayeredTexture = lProperty.GetSrcObject<FbxLayeredTexture>(j);

			//そのブレンドテクスチャのサブテクスチャの枚数を取得
			int lNbSubTextureCount = lpLayeredTexture->GetSrcObjectCount<FbxTexture>();

			//サブテクスチャそれぞれについて
			for (int k = 0; k < lNbSubTextureCount; k++)
			{
				//k番目のサブテクスチャへのアクセスの方法がわからない
			}
		}
	}
EXIT:

	return hr;
}



//Utility 

UINT FBXSDKMeshLoader::GetTrianglePolygonCount(FbxMesh* pFbxMesh)
{
	//ポリゴン数（これは四角形ポリゴンも含まれている）
	int polygonCount = pFbxMesh->GetPolygonCount();

	int polyCount = 0;
	for (int i = 0; i < polygonCount; i++)
	{
		//i番目のポリゴンを取得
		int polygonVertexSize = pFbxMesh->GetPolygonSize(i);

		//三角形ポリゴン
		if (polygonVertexSize == 3)
			polyCount += 1;

		//四角形ポリゴンなら3角形ポリゴンに分割
		else if (polygonVertexSize == 4)
			polyCount += 2;
	}
	return polyCount;
}

UINT FBXSDKMeshLoader::GetIndexCount(FbxMesh* pFbxMesh, int polygonCount)
{
	int indexCount = 0;
	for (int i = 0; i < polygonCount; i++)
	{
		//i番目のポリゴンを取得
		int polygonVertexSize = pFbxMesh->GetPolygonSize(i);

		//三角形ポリゴン
		if (polygonVertexSize == 3)
			indexCount += 3;

		//四角形ポリゴンなら3角形ポリゴンに分割
		else if (polygonVertexSize == 4)
			indexCount += 6;
	}
	return indexCount;
}

void FBXSDKMeshLoader::OutputDebugInfo(Mesh* pMesh, int vertexCount, int polygonCount, int indexCount)
{
	//DEUBG OUTPUT
	TCHAR s[512];

	//FBX関連
	{
		OutputDebugString(_T("\n\n### FBX RAW DATA \n"));

		//FBX 頂点数
		_stprintf_s(s, 512, _T("### vertex Count [%d]\n"), vertexCount);
		OutputDebugString(s);
		//FBX ポリゴン数
		_stprintf_s(s, 512, _T("### Polygon Count [%d]\n"), polygonCount);
		OutputDebugString(s);
		//FBX 頂点インデックス数
		_stprintf_s(s, 512, _T("### index Count [%d]\n\n"), indexCount);
		OutputDebugString(s);

	}

	//バッファ関連
	{
		OutputDebugString(_T("### BUFFER DATA \n"));

		//頂点バッファの頂点数
		_stprintf_s(s, 512, _T("\### Vertex Count [%d]\n"), pMesh->VertexCount);
		OutputDebugString(s);
		for (int i = 0; i < pMesh->VertexCount; i++)
		{
			_stprintf_s(s, 512, _T("###   Vertex ([%f], [%f], [%f]) "),
				pMesh->Vertexes[i].Vertex.x,
				pMesh->Vertexes[i].Vertex.y,
				pMesh->Vertexes[i].Vertex.z);
			OutputDebugString(s);

			_stprintf_s(s, 512, _T("###   Normal ([%f], [%f], [%f]) "),
				pMesh->Vertexes[i].Normal.x,
				pMesh->Vertexes[i].Normal.y,
				pMesh->Vertexes[i].Normal.z);
			OutputDebugString(s);

			_stprintf_s(s, 512, _T("###   UV ([%f], [%f])\n"),
				pMesh->Vertexes[i].UV.x,
				pMesh->Vertexes[i].UV.y);
			OutputDebugString(s);
		}

		_stprintf_s(s, 512, _T("### Index Count [%d]\n"), pMesh->IndexCount);
		OutputDebugString(s);
		OutputDebugString(_T("### "));
		for (int i = 0; i < pMesh->IndexCount; i++) {
			_stprintf_s(s, 512, _T("[%d],"), pMesh->Indexes[i]);
			OutputDebugString(s);
		}
	}
	OutputDebugString(_T("\n"));
}


//Primitive
/*
HRESULT FBXSDKMeshLoader::SetVertexBufferDebugBox(FbxMesh* pFbxMesh, Mesh* pMesh){
	TCHAR debugStr[512];
	HRESULT hr = E_FAIL;

	extern D3D11User* g_pD3D11User;

	//デバッグ用 ***************************************************

	Mesh::GVERTEX vtx[] =
	{
		{ D3DXVECTOR4(1.0f, 1.0f, -1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR4(1.0f, -1.0f, -1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(-1.0f, -1.0f, -1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f) },
		{ D3DXVECTOR4(-1.0f, 1.0f, -1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f) },

		{ D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR4(1.0f, -1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR4(-1.0f, -1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR4(-1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f) },
	};
	pMesh->VertexCount = 8;
	pMesh->Vertexes = vtx;

	//頂点バッファの作成
	hr = g_pD3D11User->CreateVertexBuffer(&(pMesh->VertexBuffer), vtx, sizeof(vtx), 0);
	if (FAILED(hr)){
		OutputDebugString(_T("ERROR : Vertex Buffer creation failed\n"));
		return E_FAIL;
		//goto EXIT;
	}

	//インデックスバッファを作成
	UINT index[] = {
		0, 2, 1,
		0, 3, 2,

		3, 7, 6,
		3, 6, 2,

		7, 4, 5,
		7, 5, 6,

		4, 0, 5,
		0, 1, 5,

		7, 3, 0,
		7, 0, 4,

		2, 6, 1,
		1, 6, 5,
	};
	pMesh->IndexCount = 36;
	pMesh->Indexes = index;
	hr = g_pD3D11User->CreateIndexBuffer(&(pMesh->IndexBuffer), index, sizeof(index), 0);
	if (FAILED(hr)){
		OutputDebugString(_T("ERROR : Index Buffer creation failed\n"));
		goto EXIT;
	}

	//DEUBG OUTPUT
	OutputDebugInfo(pMesh, 0, 0, 0);

EXIT:

	return hr;
}

HRESULT FBXSDKMeshLoader::SetVertexBufferDebugTriangle(FbxMesh* pFbxMesh, Mesh* pMesh){
	TCHAR debugStr[512];
	HRESULT hr = E_FAIL;

	extern D3D11User* g_pD3D11User;

	//デバッグ用 ***************************************************

	Mesh::GVERTEX vtx[] =
	{
		{ D3DXVECTOR4(-1.0f, -1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f) },
		{ D3DXVECTOR4(1.0f, -1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f) }
	};
	pMesh->VertexCount = 3;
	pMesh->Vertexes = vtx;

	//頂点バッファの作成
	hr = g_pD3D11User->CreateVertexBuffer(&(pMesh->VertexBuffer), vtx, sizeof(Mesh::GVERTEX) * pMesh->VertexCount, 0);
	if (FAILED(hr)){
		OutputDebugString(_T("ERROR : Vertex Buffer creation failed\n"));
		return E_FAIL;
		//goto EXIT;
	}

	//インデックスバッファを作成
	UINT index[] = {
		0, 1, 2
	};
	pMesh->IndexCount = 3;
	pMesh->Indexes = index;
	hr = g_pD3D11User->CreateIndexBuffer(&(pMesh->IndexBuffer), index, pMesh->IndexCount * sizeof(UINT), 0);
	if (FAILED(hr)){
		OutputDebugString(_T("ERROR : Index Buffer creation failed\n"));
		goto EXIT;
	}

	//DEUBG OUTPUT
	OutputDebugInfo(pMesh, 0, 0, 0);

EXIT:

	return hr;
}

HRESULT FBXSDKMeshLoader::SetVertexBufferDebugQuad(FbxMesh* pFbxMesh, Mesh* pMesh){
	TCHAR debugStr[512];
	HRESULT hr = E_FAIL;

	extern D3D11User* g_pD3D11User;

	//デバッグ用 ***************************************************

	Mesh::GVERTEX vtx[] =
	{
		{ D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f) },
		{ D3DXVECTOR4(-1.0f, -1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(1.0f, -1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f) },

		{ D3DXVECTOR4(-1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f) },
		{ D3DXVECTOR4(-2.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(-2.0f, -1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f) }
	};
	pMesh->VertexCount = 6;
	pMesh->Vertexes = vtx;

	//頂点バッファの作成
	hr = g_pD3D11User->CreateVertexBuffer(&(pMesh->VertexBuffer), vtx, sizeof(Mesh::GVERTEX) * pMesh->VertexCount, 0);
	if (FAILED(hr)){
		OutputDebugString(_T("ERROR : Vertex Buffer creation failed\n"));
		return E_FAIL;
		//goto EXIT;
	}

	//インデックスバッファを作成
	UINT index[] = {
		0, 1, 2,
		3, 4, 5
	};
	pMesh->IndexCount = 6;
	pMesh->Indexes = index;
	hr = g_pD3D11User->CreateIndexBuffer(&(pMesh->IndexBuffer), index, pMesh->IndexCount * sizeof(UINT), 0);
	if (FAILED(hr)){
		OutputDebugString(_T("ERROR : Index Buffer creation failed\n"));
		goto EXIT;
	}

	//DEUBG OUTPUT
	OutputDebugInfo(pMesh, 0, 0, 0);

EXIT:

	return hr;
}

*/
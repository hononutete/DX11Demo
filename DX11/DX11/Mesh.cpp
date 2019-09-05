#include "Mesh.h"
#include "Utility.h"

Mesh::Mesh()
{
	m_AnimationTimeSecond = 0;
	isAnimating = false;
	isLoop = true;
}


Mesh::~Mesh()
{
}

void Mesh::UpdateAnimationTime()
{
	TCHAR debugStr[512];

	//終了時間を超えていた場合
	if (m_AnimationTimeSecond > m_AnimStop){
		//ループなら時間をリセット
		if (isLoop){
			m_AnimationTimeSecond = 0;
		}
	}
	//時間を更新する
	else{
		m_AnimationTimeSecond += (1.0f / 60.0) * m_animationSpeed;
	}

	//Fbx時間に時間を設定
	m_Time.SetSecondDouble(m_AnimationTimeSecond);
	
	//_stprintf_s(debugStr, 512, _T(" Time [%lf]\n"), m_AnimationTimeSecond);
	//OutputDebugString(debugStr);
	
	//メッシュの更新
	ApplyMeshAtTime();

}

void Mesh::SetAnimation(int animStackIndex)
{
	TCHAR debugStr[512];

	//アニメーションスタック数
	int animStackCount = m_pFbxScene->GetSrcObjectCount<FbxAnimStack>();
	_stprintf_s(debugStr, 512, _T(" --- AnimStack Count [%d]\n"), animStackCount);
	OutputDebugString(debugStr);

	//アニメーションがない場合はスキップ
	if (animStackCount == 0)
		return;

	//インデックスで指定したアニメーションスタック（アニメーション）を取得
	FbxAnimStack* stack = m_pFbxScene->GetSrcObject<FbxAnimStack>(animStackIndex);

	//名前取得
	TCHAR tAnimStackName[512];
    Utility::ConvertCharToTChar(tAnimStackName, stack->GetName());

	//開始時刻と終了時刻
	m_AnimStart = stack->LocalStart.Get().GetSecondDouble();
	m_AnimStop = stack->LocalStop.Get().GetSecondDouble();

	//出力
	_stprintf_s(debugStr, 512, _T(" --- AnimStack Name [%s]\n"), tAnimStackName);
	OutputDebugString(debugStr);


	_stprintf_s(debugStr, 512, _T("     Start Time [%lf]\n"), m_AnimStart);
	OutputDebugString(debugStr);

	_stprintf_s(debugStr, 512, _T("     Stop Time [%lf]\n"), m_AnimStop);
	OutputDebugString(debugStr);

	//アニメーションスタックをシーンにセット
	m_pFbxScene->SetCurrentAnimationStack(stack);
}

void Mesh::ApplyMeshAtTime()
{
	extern D3D11User *g_pD3D11User;
	TCHAR debugStr[512];
	HRESULT hr = E_FAIL;

	int lClusterCount = 0;
	int lSkinCount = 0;

	//スキンの数を取得（これが０の場合はボーンが入っていない）
	lSkinCount = m_pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);

	//_stprintf_s(debugStr, 512, _T(" Skin Count [%d]\n"), lSkinCount);
	//OutputDebugString(debugStr);

	//それぞれのスキンについて
	for (int i = 0; i < lSkinCount; i++)
	{
		
		//i番目のスキンを取得
		FbxSkin* lFbxSkin = (FbxSkin*)m_pFbxMesh->GetDeformer(i, FbxDeformer::eSkin);

		//スキンに含まれるボーンの数を取得
		lClusterCount = lFbxSkin->GetClusterCount();
		//_stprintf_s(debugStr, 512, _T(" Skin [%d]\n"), i);
		//OutputDebugString(debugStr);
		//_stprintf_s(debugStr, 512, _T(" --- Bone Count [%d]\n"), lClusterCount);
		//OutputDebugString(debugStr);

		//頂点の移動量を初期姿勢行列とボーン行列から計算するため、最初は0に設定する
		for (int k = 0; k < VertexCount; k++) {
			Vertexes[k].Vertex.x = 0;
			Vertexes[k].Vertex.y = 0;
			Vertexes[k].Vertex.z = 0;
			Vertexes[k].Vertex.w = 1;
		}

		//それぞれのボーンについて
		//頂点に対するウェイトは合計で１にならなければならない
		//必要な計算は頂点の初期行列＋フレーム行列（ウェイトの合計）
		for (int j = 0; j < lClusterCount; j++)
		{
			
			//j番目のボーンを取得
			FbxCluster* lCluster = lFbxSkin->GetCluster(j);

			int lIndexCount = lCluster->GetControlPointIndicesCount(); //ボーンに紐づけられている頂点数
			int* lIndices = lCluster->GetControlPointIndices();        //ボーンに紐づけられているコントロールポイントのインデックス配列
			double* lWeights = lCluster->GetControlPointWeights();    //ボーンに紐づけられている頂点のウェイト

			//_stprintf_s(debugStr, 512, _T("\n --- Bone [%d] vertex count [%d]\n"), j, lIndexCount);
			//OutputDebugString(debugStr);
			
			//初期姿勢行列
			FbxAMatrix initMatFbx;
			lCluster->GetTransformLinkMatrix(initMatFbx);
			
			
			FbxVector4 lrowA = initMatFbx.GetRow(0);
			FbxVector4 lrowB = initMatFbx.GetRow(1);
			FbxVector4 lrowC = initMatFbx.GetRow(2);
			FbxVector4 lrowD = initMatFbx.GetRow(3);

			//_stprintf_s(debugStr, 512, _T(" --- Init Matrix ( [%f], [%f], [%f], [%f] ) \n"), lrowA[0], lrowA[1], lrowA[2], lrowA[3]);
			//OutputDebugString(debugStr);
			//_stprintf_s(debugStr, 512, _T("                 ( [%f], [%f], [%f], [%f] ) \n"), lrowB[0], lrowB[1], lrowB[2], lrowB[3]);
			//OutputDebugString(debugStr);
			//_stprintf_s(debugStr, 512, _T("                 ( [%f], [%f], [%f], [%f] ) \n"), lrowC[0], lrowC[1], lrowC[2], lrowC[3]);
			//OutputDebugString(debugStr);
			//_stprintf_s(debugStr, 512, _T("                 ( [%f], [%f], [%f], [%f] ) \n"), lrowD[0], lrowD[1], lrowD[2], lrowD[3]);
			//OutputDebugString(debugStr);
			

			//フレーム行列
			FbxNode* node = lCluster->GetLink(); //ノードを取得
			XMMATRIX frameMatrix = XMMatrixIdentity();
			FbxAMatrix frameMatrixFbx = node->EvaluateGlobalTransform(m_Time);
			frameMatrix = Utility::ConvertFbxMatrixToXMMatrix(frameMatrixFbx);

			/*
			//FbxVector4 lrowAf = initMat.GetRow(0);
			//FbxVector4 lrowBf = initMat.GetRow(1);
			//FbxVector4 lrowCf = initMat.GetRow(2);
			//FbxVector4 lrowDf = initMat.GetRow(3);

			//_stprintf_s(debugStr, 512, _T(" --- Frame Matrix ( [%f], [%f], [%f], [%f] ) \n"), lrowAf[0], lrowAf[1], lrowAf[2], lrowAf[3]);
			//OutputDebugString(debugStr);
			//_stprintf_s(debugStr, 512, _T("                 ( [%f], [%f], [%f], [%f] ) \n"), lrowBf[0], lrowBf[1], lrowBf[2], lrowBf[3]);
			//OutputDebugString(debugStr);
			//_stprintf_s(debugStr, 512, _T("                 ( [%f], [%f], [%f], [%f] ) \n"), lrowCf[0], lrowCf[1], lrowCf[2], lrowCf[3]);
			//OutputDebugString(debugStr);
			//_stprintf_s(debugStr, 512, _T("                 ( [%f], [%f], [%f], [%f] ) \n"), lrowDf[0], lrowDf[1], lrowDf[2], lrowDf[3]);
			//OutputDebugString(debugStr);
			*/

			//初期姿勢は逆行列として使う
			XMMATRIX initMat = XMMatrixIdentity();
			initMat = Utility::ConvertFbxMatrixToXMMatrix(initMatFbx);
			XMMATRIX initMatInv = XMMatrixIdentity();
			initMatInv = XMMatrixInverse (nullptr, initMat);
			
			//最終的なフレーム行列
			XMMATRIX frameTransformMatrix = XMMatrixIdentity();
			//frameTransformMatrix = initMatInv * frameMatrix;

			//頂点を取得
			for (int k = 0; k < lIndexCount; k++)
			{
				//コントロールポイントのインデックス
				int controlPointIndex = lIndices[k];

				//頂点ウェイト
				double weight = lWeights[k];

				//頂点の移動量にウェイトを適用
				XMMATRIX frameMatrixWeighted = XMMatrixIdentity();
				frameMatrixWeighted = frameMatrix * weight;
				frameTransformMatrix = initMatInv * frameMatrixWeighted;

				//frameTransformMatrix *= weight; //TODO:ウェイトをすべての要素にかけていいのか？

				//_stprintf_s(debugStr, 512, _T("     --- controlpoint Index [%d]\n"), controlPointIndex);
				//OutputDebugString(debugStr);
				//_stprintf_s(debugStr, 512, _T("     --- weight [%lf]\n"), weight);
				//OutputDebugString(debugStr);

				//*** 頂点変換 ***
				
				//i番目のポリゴン内のj番目の頂点の座標　（ローカル座標）
				XMFLOAT4 vec4 = Utility::ConvertFbxVectorToXMFLOAT4(m_pFbxMesh->GetControlPointAt(controlPointIndex));
				XMVECTOR vec = XMLoadFloat4(&vec4);
				
				//D3DXVECTOR4 vec4 = Utility::ConvertFbxVectorToD3DXVector(m_pFbxMesh->GetControlPointAt(controlPointIndex));
				XMFLOAT4 framePosition;
				
				XMVECTOR framePos = XMVector4Transform(vec, frameTransformMatrix);
				XMFLOAT4 vecf;
				XMStoreFloat4(&vecf, framePos);
				//ローカル座標に、最終的なフレーム行列をかける（初期姿勢行列の逆行列をかけて、フレーム行列をかけたのが頂点位置）
				//D3DXVec4Transform(&framePosition, &vec4, &frameTransformMatrix);

				//分割後のコントロールポイントに頂点座標を適用
				for (int q = 0; q < (controlPointRef[controlPointIndex]).size(); q++){
					int splitIndex = controlPointRef[controlPointIndex][q];
					Vertexes[splitIndex].Vertex.x += vecf.x;
					Vertexes[splitIndex].Vertex.y += vecf.y;
					Vertexes[splitIndex].Vertex.z += vecf.z;
					//Vertexes[splitIndex].Vertex = Vertexes[splitIndex].Vertex;

					//_stprintf_s(debugStr, 512, _T("     --- ---> split Index [%d]\n"), splitIndex);
					//OutputDebugString(debugStr);
				}

			}

		}
		//とりあえずスキンはひとつのみサポート
		break;
	}

	//頂点バッファの頂点を更新
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pD3D11User->m_D3DDeviceContext->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, Vertexes, sizeof(GVERTEX)*VertexCount);
	g_pD3D11User->m_D3DDeviceContext->Unmap(VertexBuffer, 0);

}

bool Mesh::HasAnimation() 
{
	return m_pFbxMesh != nullptr;
}
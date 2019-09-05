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

	//�I�����Ԃ𒴂��Ă����ꍇ
	if (m_AnimationTimeSecond > m_AnimStop){
		//���[�v�Ȃ玞�Ԃ����Z�b�g
		if (isLoop){
			m_AnimationTimeSecond = 0;
		}
	}
	//���Ԃ��X�V����
	else{
		m_AnimationTimeSecond += (1.0f / 60.0) * m_animationSpeed;
	}

	//Fbx���ԂɎ��Ԃ�ݒ�
	m_Time.SetSecondDouble(m_AnimationTimeSecond);
	
	//_stprintf_s(debugStr, 512, _T(" Time [%lf]\n"), m_AnimationTimeSecond);
	//OutputDebugString(debugStr);
	
	//���b�V���̍X�V
	ApplyMeshAtTime();

}

void Mesh::SetAnimation(int animStackIndex)
{
	TCHAR debugStr[512];

	//�A�j���[�V�����X�^�b�N��
	int animStackCount = m_pFbxScene->GetSrcObjectCount<FbxAnimStack>();
	_stprintf_s(debugStr, 512, _T(" --- AnimStack Count [%d]\n"), animStackCount);
	OutputDebugString(debugStr);

	//�A�j���[�V�������Ȃ��ꍇ�̓X�L�b�v
	if (animStackCount == 0)
		return;

	//�C���f�b�N�X�Ŏw�肵���A�j���[�V�����X�^�b�N�i�A�j���[�V�����j���擾
	FbxAnimStack* stack = m_pFbxScene->GetSrcObject<FbxAnimStack>(animStackIndex);

	//���O�擾
	TCHAR tAnimStackName[512];
    Utility::ConvertCharToTChar(tAnimStackName, stack->GetName());

	//�J�n�����ƏI������
	m_AnimStart = stack->LocalStart.Get().GetSecondDouble();
	m_AnimStop = stack->LocalStop.Get().GetSecondDouble();

	//�o��
	_stprintf_s(debugStr, 512, _T(" --- AnimStack Name [%s]\n"), tAnimStackName);
	OutputDebugString(debugStr);


	_stprintf_s(debugStr, 512, _T("     Start Time [%lf]\n"), m_AnimStart);
	OutputDebugString(debugStr);

	_stprintf_s(debugStr, 512, _T("     Stop Time [%lf]\n"), m_AnimStop);
	OutputDebugString(debugStr);

	//�A�j���[�V�����X�^�b�N���V�[���ɃZ�b�g
	m_pFbxScene->SetCurrentAnimationStack(stack);
}

void Mesh::ApplyMeshAtTime()
{
	extern D3D11User *g_pD3D11User;
	TCHAR debugStr[512];
	HRESULT hr = E_FAIL;

	int lClusterCount = 0;
	int lSkinCount = 0;

	//�X�L���̐����擾�i���ꂪ�O�̏ꍇ�̓{�[���������Ă��Ȃ��j
	lSkinCount = m_pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);

	//_stprintf_s(debugStr, 512, _T(" Skin Count [%d]\n"), lSkinCount);
	//OutputDebugString(debugStr);

	//���ꂼ��̃X�L���ɂ���
	for (int i = 0; i < lSkinCount; i++)
	{
		
		//i�Ԗڂ̃X�L�����擾
		FbxSkin* lFbxSkin = (FbxSkin*)m_pFbxMesh->GetDeformer(i, FbxDeformer::eSkin);

		//�X�L���Ɋ܂܂��{�[���̐����擾
		lClusterCount = lFbxSkin->GetClusterCount();
		//_stprintf_s(debugStr, 512, _T(" Skin [%d]\n"), i);
		//OutputDebugString(debugStr);
		//_stprintf_s(debugStr, 512, _T(" --- Bone Count [%d]\n"), lClusterCount);
		//OutputDebugString(debugStr);

		//���_�̈ړ��ʂ������p���s��ƃ{�[���s�񂩂�v�Z���邽�߁A�ŏ���0�ɐݒ肷��
		for (int k = 0; k < VertexCount; k++) {
			Vertexes[k].Vertex.x = 0;
			Vertexes[k].Vertex.y = 0;
			Vertexes[k].Vertex.z = 0;
			Vertexes[k].Vertex.w = 1;
		}

		//���ꂼ��̃{�[���ɂ���
		//���_�ɑ΂���E�F�C�g�͍��v�łP�ɂȂ�Ȃ���΂Ȃ�Ȃ�
		//�K�v�Ȍv�Z�͒��_�̏����s��{�t���[���s��i�E�F�C�g�̍��v�j
		for (int j = 0; j < lClusterCount; j++)
		{
			
			//j�Ԗڂ̃{�[�����擾
			FbxCluster* lCluster = lFbxSkin->GetCluster(j);

			int lIndexCount = lCluster->GetControlPointIndicesCount(); //�{�[���ɕR�Â����Ă��钸�_��
			int* lIndices = lCluster->GetControlPointIndices();        //�{�[���ɕR�Â����Ă���R���g���[���|�C���g�̃C���f�b�N�X�z��
			double* lWeights = lCluster->GetControlPointWeights();    //�{�[���ɕR�Â����Ă��钸�_�̃E�F�C�g

			//_stprintf_s(debugStr, 512, _T("\n --- Bone [%d] vertex count [%d]\n"), j, lIndexCount);
			//OutputDebugString(debugStr);
			
			//�����p���s��
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
			

			//�t���[���s��
			FbxNode* node = lCluster->GetLink(); //�m�[�h���擾
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

			//�����p���͋t�s��Ƃ��Ďg��
			XMMATRIX initMat = XMMatrixIdentity();
			initMat = Utility::ConvertFbxMatrixToXMMatrix(initMatFbx);
			XMMATRIX initMatInv = XMMatrixIdentity();
			initMatInv = XMMatrixInverse (nullptr, initMat);
			
			//�ŏI�I�ȃt���[���s��
			XMMATRIX frameTransformMatrix = XMMatrixIdentity();
			//frameTransformMatrix = initMatInv * frameMatrix;

			//���_���擾
			for (int k = 0; k < lIndexCount; k++)
			{
				//�R���g���[���|�C���g�̃C���f�b�N�X
				int controlPointIndex = lIndices[k];

				//���_�E�F�C�g
				double weight = lWeights[k];

				//���_�̈ړ��ʂɃE�F�C�g��K�p
				XMMATRIX frameMatrixWeighted = XMMatrixIdentity();
				frameMatrixWeighted = frameMatrix * weight;
				frameTransformMatrix = initMatInv * frameMatrixWeighted;

				//frameTransformMatrix *= weight; //TODO:�E�F�C�g�����ׂĂ̗v�f�ɂ����Ă����̂��H

				//_stprintf_s(debugStr, 512, _T("     --- controlpoint Index [%d]\n"), controlPointIndex);
				//OutputDebugString(debugStr);
				//_stprintf_s(debugStr, 512, _T("     --- weight [%lf]\n"), weight);
				//OutputDebugString(debugStr);

				//*** ���_�ϊ� ***
				
				//i�Ԗڂ̃|���S������j�Ԗڂ̒��_�̍��W�@�i���[�J�����W�j
				XMFLOAT4 vec4 = Utility::ConvertFbxVectorToXMFLOAT4(m_pFbxMesh->GetControlPointAt(controlPointIndex));
				XMVECTOR vec = XMLoadFloat4(&vec4);
				
				//D3DXVECTOR4 vec4 = Utility::ConvertFbxVectorToD3DXVector(m_pFbxMesh->GetControlPointAt(controlPointIndex));
				XMFLOAT4 framePosition;
				
				XMVECTOR framePos = XMVector4Transform(vec, frameTransformMatrix);
				XMFLOAT4 vecf;
				XMStoreFloat4(&vecf, framePos);
				//���[�J�����W�ɁA�ŏI�I�ȃt���[���s���������i�����p���s��̋t�s��������āA�t���[���s����������̂����_�ʒu�j
				//D3DXVec4Transform(&framePosition, &vec4, &frameTransformMatrix);

				//������̃R���g���[���|�C���g�ɒ��_���W��K�p
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
		//�Ƃ肠�����X�L���͂ЂƂ̂݃T�|�[�g
		break;
	}

	//���_�o�b�t�@�̒��_���X�V
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pD3D11User->m_D3DDeviceContext->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, Vertexes, sizeof(GVERTEX)*VertexCount);
	g_pD3D11User->m_D3DDeviceContext->Unmap(VertexBuffer, 0);

}

bool Mesh::HasAnimation() 
{
	return m_pFbxMesh != nullptr;
}
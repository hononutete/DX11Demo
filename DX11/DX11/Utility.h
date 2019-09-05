#pragma once
#ifndef FBXSDK_SHARED
#define FBXSDK_SHARED
#endif 

#include <d3d11.h>
#include <fbxsdk.h>
#include <DirectXMath.h>

#include <vector>;

using namespace DirectX;
class Utility{
public:
	static char* str_join(const char* a, const char* b);

	static void ConvertCharToTChar(TCHAR pDestStr[512], const char* pSrcStr);

	//static void Utility::ConvertFbxMatrixToD3DXMatrix(FbxMatrix matrix, D3DXMATRIX &dxMatrix);
	static XMMATRIX ConvertFbxMatrixToXMMatrix(FbxMatrix matrix);

	static XMFLOAT4 ConvertFbxVectorToXMFLOAT4(FbxVector4 vector);

	static int VectorFindElementTest();

	template<class T>
	static int VectorFindElement(std::vector<T> &vec, T element)
	{
		//�ŏ���element�C�e���[�^�[��Ԃ�
		auto itr = std::find(vec.begin(), vec.end(), element);

		//������Ȃ������ꍇ�A�Ō�̃C�e���[�^�[���Ԃ�
		if (itr == vec.end())
		{
			//������Ȃ������ꍇ-1��Ԃ�
			return -1;
		}
		//���������ꍇ�A�Ō�ł͂Ȃ��C�e���[�^�[��������
		else
		{
			//���������ꍇ�A�C���f�b�N�X��Ԃ�
			return itr - vec.begin();
		}
	}

	template<class T>
	static int VectorFindElement(T element)
	{
		return -1;
	}

	

	//static D3DXMATRIX ConvertFbxMatrixToD3DXMatrix(FbxMatrix matrix); //�Q�ƈ������g���ׂ����H

	//static D3DXVECTOR4 ConvertFbxVectorToD3DXVector(FbxVector4 vector); //�Q�ƈ������g���ׂ����H

};
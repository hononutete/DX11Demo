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
		//最初のelementイテレーターを返す
		auto itr = std::find(vec.begin(), vec.end(), element);

		//見つからなかった場合、最後のイテレーターが返る
		if (itr == vec.end())
		{
			//見つからなかった場合-1を返す
			return -1;
		}
		//見つかった場合、最後ではないイテレーターがかえる
		else
		{
			//見つかった場合、インデックスを返す
			return itr - vec.begin();
		}
	}

	template<class T>
	static int VectorFindElement(T element)
	{
		return -1;
	}

	

	//static D3DXMATRIX ConvertFbxMatrixToD3DXMatrix(FbxMatrix matrix); //参照引数を使うべきか？

	//static D3DXVECTOR4 ConvertFbxVectorToD3DXVector(FbxVector4 vector); //参照引数を使うべきか？

};
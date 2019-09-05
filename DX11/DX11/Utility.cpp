#include "Utility.h";
#include <string.h>;

char* Utility::str_join(const char* a, const char* b){
	size_t la = strlen(a);
	size_t lb = strlen(b);
	char* p = new char[la + lb];
	memcpy(p, a, la + 1);
	memcpy(p + la, b, lb + 1);
	return p;
}

void Utility::ConvertCharToTChar(TCHAR pDestStr[512], const char* pSrcStr){
#ifdef _UNICODE
	// 第一引数で指定した文字コードを Unicode 文字コードに変換する
	::MultiByteToWideChar(CP_UTF8, 0, pSrcStr, -1, pDestStr, (int)((strlen(pSrcStr) + 1) * sizeof(WCHAR)));
#else
	WCHAR str[512];
	// 第一引数で指定した文字コードを Unicode 文字コードに変換する
	::MultiByteToWideChar(CP_UTF8, 0, pSrcStr, -1, str, (int)((strlen(pSrcStr) + 1) * sizeof(WCHAR)));
	// Unicode 文字コードを第一引数で指定した文字コードに変換する( CP_ACP は日本語WindowdではシフトJISコード )
	::WideCharToMultiByte(CP_ACP, 0, str, -1, pDestStr, (int)((wcslen(str) + 1) * 2), NULL, NULL);
#endif
}

DirectX::XMMATRIX Utility::ConvertFbxMatrixToXMMatrix(FbxMatrix matrix) {
	XMMATRIX xmmat = XMMATRIX(
		matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
		matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
		matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
		matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]
	);
	return xmmat;
}

DirectX::XMFLOAT4 Utility::ConvertFbxVectorToXMFLOAT4(FbxVector4 vector) {
	XMFLOAT4 vec = XMFLOAT4(vector[0], vector[1], vector[2], vector[3]);
	return vec;
}

int Utility::VectorFindElementTest() {
	return -1;
}

//template<class T>
//int Utility::VectorFindElement(std::vector<T> &vec, T element) 
//{
//	//最初のelementイテレーターを返す
//	auto itr = std::find(vec.begin(), v.end(), element);
//
//	//見つからなかった場合、最後のイテレーターが返る
//	if (itr == vec.end()) 
//	{
//		//見つからなかった場合-1を返す
//		return -1;
//	}
//	//見つかった場合、最後ではないイテレーターがかえる
//	else 
//	{
//		//見つかった場合、インデックスを返す
//		return itr - vec.begin();
//	}
//}

//template<class T>
//int Utility::VectorFindElement(T element)
//{
//	return -1;
//}
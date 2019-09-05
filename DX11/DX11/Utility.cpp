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
	// �������Ŏw�肵�������R�[�h�� Unicode �����R�[�h�ɕϊ�����
	::MultiByteToWideChar(CP_UTF8, 0, pSrcStr, -1, pDestStr, (int)((strlen(pSrcStr) + 1) * sizeof(WCHAR)));
#else
	WCHAR str[512];
	// �������Ŏw�肵�������R�[�h�� Unicode �����R�[�h�ɕϊ�����
	::MultiByteToWideChar(CP_UTF8, 0, pSrcStr, -1, str, (int)((strlen(pSrcStr) + 1) * sizeof(WCHAR)));
	// Unicode �����R�[�h��������Ŏw�肵�������R�[�h�ɕϊ�����( CP_ACP �͓��{��Windowd�ł̓V�t�gJIS�R�[�h )
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
//	//�ŏ���element�C�e���[�^�[��Ԃ�
//	auto itr = std::find(vec.begin(), v.end(), element);
//
//	//������Ȃ������ꍇ�A�Ō�̃C�e���[�^�[���Ԃ�
//	if (itr == vec.end()) 
//	{
//		//������Ȃ������ꍇ-1��Ԃ�
//		return -1;
//	}
//	//���������ꍇ�A�Ō�ł͂Ȃ��C�e���[�^�[��������
//	else 
//	{
//		//���������ꍇ�A�C���f�b�N�X��Ԃ�
//		return itr - vec.begin();
//	}
//}

//template<class T>
//int Utility::VectorFindElement(T element)
//{
//	return -1;
//}
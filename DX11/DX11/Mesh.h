#pragma once

#ifndef FBXSDK_SHARED
#define FBXSDK_SHARED
#endif 


#include "UCommon.h"
#include <vector>;
#include <unordered_map>;
#include <d3d11.h>;
#include "D3D11User.h";
#include <string.h>;
#include <stdio.h> ; 
#include <stdlib.h>;  
#include <fbxsdk.h>;
#include <DirectXMath.h>
using namespace DirectX;

class Mesh
{
public:

	//���_�\���̐錾
	typedef struct t_Vertex{
		XMFLOAT4 Vertex;
		XMFLOAT3 Normal;
		XMFLOAT4 Color;
		XMFLOAT2 UV;
	} GVERTEX;


	//�e�N�X�`���\���̐錾
	typedef struct t_Texture{
		TCHAR MaterialName[512];
		TCHAR TexturePath[512];
		ID3D11ShaderResourceView* Texture;
	} TEXTURE;

	//�{�[���\����
	typedef struct t_Bone{
		XMMATRIX initMatrix;
		int indexCount;
		int* indices;
	};

	GVERTEX* Vertexes;  //���_���W�z��
	UINT VertexCount;  //���_��
	ID3D11Buffer* VertexBuffer; //���_�o�b�t�@

	UINT* Indexes;  //���_�C���f�b�N�X�z��
	UINT IndexCount;//���_�C���f�b�N�X��
	ID3D11Buffer* IndexBuffer; //�C���f�b�N�X�o�b�t�@

	std::vector<TEXTURE> Textures; //���b�V���Ŏg���e�N�X�`���z��

	//�ʖ@�����g�p����ꍇ�A���Ƃ̃R���g���[���|�C���g�ƁA������ɂǂ̃R���g���[���|�C���g�ɕ������ꂽ���H�̎Q�Ƃ�ێ�����n�b�V���e�[�u��
	std::unordered_map<int, std::vector<int>> controlPointRef;


	//*** �A�j���[�V�����֘A ***

	//�A�j���[�V�����\���͏����I�ɕ�������
	FbxMesh* m_pFbxMesh;
	FbxScene* m_pFbxScene;


	FbxTime m_Time;
	bool isAnimating;
	//�A�j���[�V�����o�ߎ��ԁi�b�j
	double m_AnimationTimeSecond;
	//���[�v�����邩�ǂ���
	bool isLoop;
	//�A�j���[�V�����J�n����
	double m_AnimStart;
	//�A�j���[�V�����I������
	double m_AnimStop;
	//�A�j���[�V�����̃X�s�[�h
	float m_animationSpeed = 0.1f;

	//�R���X�g���N�^
	Mesh();

	//�f�X�g���N�^
	~Mesh();

	void SetAnimation(int animStackIndex);
	
	//�A�j���[�V�������Ԃ��X�V
	void UpdateAnimationTime();

	void ApplyMeshAtTime();

	bool HasAnimation();

	//*** �n���� ***
	//ID3D11ShaderResourceView* pSRV = NULL;�@//�|�C���^�ϐ����쐬	
	//g_pBoxMesh->MeshUser[DrawMeshIndex].GetTexture(_T("DiffuseColor"), &pSRV);
	//g_pD3D11User->m_D3DDeviceContext->PSSetShaderResources(0, 1, &pSRV);
	//pSRV ���@�A�h���X�������Ă���i64fdf0�Ȃǁj
	//*pSRV�@���@�A�h���X���w�����g
	//�@�����ł̓|�C���^�̃A�h���X�������ɓn���A�󂯎����ł̓|�C���^�̃|�C���^�Ƃ��Ă����Ƃ�
	//�}�e���A���̖��O�Ńe�N�X�`�����擾
	void GetTexture(const TCHAR* pMaterialName, ID3D11ShaderResourceView** ppSRV)
	{
		TCHAR debugStr[512];
		//�܂�*pSRV�Ń|�C���^�ϐ��̐錾�������B������&pSRV�������ɓn��
		//&pSRV�́A�|�C���^�ϐ��̃A�h���X��n���Ă���B
		//
		//�@�����Ŏ����I�ɕϊ�������B��̓I�ɂ͂���Ȋ����̕ϊ�
		//  ID3D11ShaderResourceView** ppSRV = &pSRV
		//
		//�܂��󂯎������Ń|�C���^�̃|�C���^�ϐ�**ppSRV��錾�A���łɃ|�C���^�ϐ�pSRV�̃A�h���X���i�[����Ă���
		//ppSRV ���@pSRV�|�C���^�ϐ��̃A�h���X
		//*ppSRV�@���@pSRV�|�C���^�ϐ��̒��g�i���Ƃ���pSRV��
		//*ppSRV�̂悤��*������Ɠ��e���Q�Ƃ���B�܂�*ppSRV�Ƃ����
		(*ppSRV) = NULL; //�|�C���^�̃|�C���^�̃A�h���X���Q�Ƃ��钆�g���N���A�i�{���ł���Β��g�ɂ̓|�C���^�ϐ��̃A�h���X������j
		                 //�|�C���^�ϐ����N���A
		for (int i = 0; i < (int)Textures.size(); i++)
		{
			//_stprintf_s(debugStr, 512, _T("   Material Name [%s]\n"), Textures[i].MaterialName);
			//OutputDebugString(debugStr);

			//��������r����A�����������ꍇ0��������
			if (_tcscmp(Textures[i].MaterialName, pMaterialName) == 0)
			{
				(*ppSRV) = Textures[i].Texture;
				break;
			}
		}
	}
};

class MeshGroup
{
public:
	Mesh* Meshes;        //�S���b�V���̔z��
	UINT  MeshCount;     //�S���b�V����

	//�R���X�g���N�^
	MeshGroup(){
		Meshes = NULL;
		MeshCount = 0;
	}

	//�f�X�g���N�^
	~MeshGroup(){
		SAFE_DELETE_ARRAY(Meshes);
	}
};


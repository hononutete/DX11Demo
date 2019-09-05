#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class Shader
{
public:
	__declspec(align(16))
	struct CB_LAMBERT {
		XMMATRIX matWVP;     //WVP�ϊ��s��
		XMMATRIX matNormal;  //�@���ϊ��s��
		XMFLOAT4 vecLight; //���s�ˉe���C�g
		float ambient;        //�������C�g
	};

	//�萔�o�b�t�@
	ID3D11Buffer* m_pConstantBuffersLambert = NULL;

	//���_���C�A�E�g�i���̓��C�A�E�g�j
	//DX11�ł̓V�F�[�_�[�̓��̓Z�}���e�B�N�X�����R�Ɍ��߂����悤�ɂȂ�������
	//���_�o�b�t�@�ƃV�F�[�_�[�����т��邽�߂ɒ��_���C�A�E�g���f�o�C�X�ɓo�^����
	ID3D11InputLayout* m_pLayout = NULL;

	//���_�V�F�[�_�[
	ID3D11VertexShader* m_pVertexShader = NULL;

	//�s�N�Z���V�F�[�_�[
	ID3D11PixelShader* m_pPixelShader = NULL;

	//�T���v���[�X�e�[�g
	ID3D11SamplerState *m_pSamplerState = NULL;


	Shader();
	~Shader();
};


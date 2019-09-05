#pragma once
//
//#ifndef DXUser_H
//#define DXUser_H

#include "UCommon.h"
//#include <windows.h>
#include <d3d11.h>
//#include <DXGI.h>

class D3D11User
{
public:
	ID3D11Device* m_D3DDevice;                   //�f�o�C�X�i�`���S������j
	IDXGIAdapter* m_Adapter;                    //�A�_�v�^�[
	ID3D11DeviceContext* m_D3DDeviceContext;     //�f�o�C�X�R���e�L�X�g�i�ݒ�l�Ȃǂ��i�[����Ă���j
	IDXGISwapChain* m_SwapChain;                 //�o�b�t�@���Ǘ�����i�o�b�N�o�b�t�@�̓X���b�v�`�F�[���̒��ɂ���j
	ID3D11RenderTargetView*  m_RenderTargetView; //���\�[�X�i�X���b�v�`�F�[���̃o�b�N�o�b�t�@�j��`��p�C�v���C���i�f�o�C�X�j�Ɍ����������
	ID3D11DepthStencilView* m_DepthStencilView;  //�[�x�X�e���V���r���[
	//HWND m_hWnd;//�E�B���h�E
	D3D_FEATURE_LEVEL m_FeatureLevel;            //�f�o�C�X�쐬���Ɏ��ۂɍ̗p���ꂽ�t�B�[�`���[���x��

	D3D11User();
	~D3D11User();

	//������
	HRESULT InitD3D11(HINSTANCE hInstance, HWND hWnd);

	//������
	HRESULT InitD3D11(WNDPROC WinProc, HINSTANCE hInstance, TCHAR* AppName);

	//�I������
	void Invalidate();

	//���_�o�b�t�@�̍쐬
	HRESULT CreateVertexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag);

	//�C���f�b�N�X�o�b�t�@�̍쐬
	HRESULT CreateIndexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag);

	//���_�V�F�[�_�[���t�@�C������쐬
	HRESULT CreateVertexShaderFromFile(ID3D11VertexShader** pVertexShader, TCHAR pSrcFile[], const CHAR pFunctionName[], const CHAR pProfile[], ID3D11InputLayout** g_pLayout, D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements);
	//HRESULT CreateVertexShaderFromFile(OUT ID3D11VertexShader** pVertexShader,
	//	IN TCHAR pSrcFile[], IN CHAR pFunctionName[], IN CHAR pProfile[],
	//	OUT ID3D11InputLayout** g_pLayout,
	//	IN D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements);

	//�s�N�Z���V�F�[�_�[���t�@�C������쐬
	HRESULT CreatePixelShaderFromFile(ID3D11PixelShader** pPixelShader, TCHAR pSrcFile[], const CHAR pFunctionName[], const CHAR pProfile[]);

	//�萔�o�b�t�@�̍쐬
	HRESULT CreateConstantBuffer( ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag);


private:

	//�E�B���h�E�̍쐬
	HRESULT InitWindow(WNDPROC WinProc, HINSTANCE hInstance, TCHAR* AppName);

	//�A�_�v�^�[�쐬
	HRESULT CreateAdapter();

	//�f�o�C�X�̍쐬
	HRESULT CreateDevice();

	//�X���b�v�`�F�[���̍쐬
	HRESULT CreateSwapChain();

	//�X���b�v�`�F�[���̍쐬
	HRESULT CreateSwapChain(HWND hWnd);

	//�[�x�X�e���V���r���[�̍쐬
	HRESULT CreateDepthStencilView();

	//�f�t�H���g�̃��X�^���C�U��ݒ�
	HRESULT SetDefaultRasterize();

	//�f�t�H���g�̐[�x�X�e���V���X�e�[�g��ݒ�
	HRESULT SetDefaultDepthStencilState();

	//�����_�[�^�[�Q�b�g�r���[�̍쐬
	HRESULT CreateRenderTargetView();

	//�r���[�|�[�g�̍쐬
	HRESULT CreateViewport();

	//�o�b�t�@�̍쐬
	HRESULT CreateBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, D3D11_BIND_FLAG BindFlag, UINT CPUAccessFlag);

};

//#endif
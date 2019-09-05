#include "stdafx.h"
#include "ShaderCompiler.h"
#include "D3D11User.h"
ShaderCompiler::ShaderCompiler()
{
	m_shaderMap.clear();
}


ShaderCompiler::~ShaderCompiler()
{
	for (auto itr = m_shaderMap.begin(); itr != m_shaderMap.end(); ++itr) {
		SAFE_DELETE(itr->second);
	}
	m_shaderMap.clear();
}

HRESULT ShaderCompiler::CompileShaders()
{
	//default diffuse shader
	Shader* defaultDiffuseShader = new Shader();
	CompileDefaultDiffuseShader (defaultDiffuseShader);
	m_shaderMap[*(_T("DefaultDiffse"))] = defaultDiffuseShader;

	//textured diffuse shader
	Shader* texturedDiffuseShader = new Shader();
	CompileTextureDiffuseShader(texturedDiffuseShader);
	m_shaderMap[*(_T("TexturedDiffuse"))] = texturedDiffuseShader;

	return S_OK;
}


HRESULT ShaderCompiler::CompileDefaultDiffuseShader(Shader* p_Shader) 
{
	HRESULT hr = E_FAIL;
	extern D3D11User* g_pD3D11User;

	// ******** ���_�V�F�[�_�[���쐬 *********** //

	//���_�v�f�̋L�q�i���_�\���̑��Z�}���e�B�N�X�j
	//�@�@�@���_�\���̂ɐ錾����Ă��鍀�ڂ̎�ށA�T�C�Y���L�q����D3D11_INPUT_ELEMENT_DESC�\����
	D3D11_INPUT_ELEMENT_DESC layout[] = {

		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//����DX11�N���X�Œ��_�V�F�[�_�[�쐬
	hr = g_pD3D11User->CreateVertexShaderFromFile(
		&(p_Shader->m_pVertexShader),  //���_�V�F�[�_�[�I�u�W�F�N�g�ւ̃|�C���^
		_T("DefaultDiffuse.hlsl"),       //�V�F�[�_�[�R�[�h���i�[����Ă���t�@�C�����i�v���W�F�N�g���[�g����̃p�X�Ŏw��j
		"VS_Main",         //�V�F�[�_�[�̎��s���J�n�����G���g���|�C���g�֐���
		"vs_4_0",          //�V�F�[�_�[���f�����w��
		&(p_Shader->m_pLayout),        //���_���C�A�E�g�ւ̃|�C���^
		layout,            //���_���C�A�E�g����邽�߂̒��_���(D3D11_INPUT_ELEMENT_DESC)
		_countof(layout)   //���_���z��
	);
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : Create vertex shader from file failed\n"));
		goto EXIT;
	}
	// ******** �s�N�Z���V�F�[�_�[���쐬 *********** //

	hr = g_pD3D11User->CreatePixelShaderFromFile(
		&(p_Shader->m_pPixelShader),   //�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�ւ̃|�C���^
		_T("DefaultDiffuse.hlsl"),       //�V�F�[�_�[�R�[�h���i�[����Ă���t�@�C�����i�v���W�F�N�g���[�g����̃p�X�Ŏw��j
		"PS_Main",         //�V�F�[�_�[�̎��s���J�n�����G���g���|�C���g�֐���
		"ps_4_0"           //�V�F�[�_�[���f�����w��
	);
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : Create pixel shader from file failed\n"));
		goto EXIT;
	}

	//**** �萔�o�b�t�@���쐬 **************************

	//�����o�[�g���˗p�萔�o�b�t�@�\���̂̒萔�o�b�t�@���쐬
	//CPUAccessFlag�̐ݒ��
	hr = g_pD3D11User->CreateConstantBuffer(&(p_Shader->m_pConstantBuffersLambert), NULL, sizeof(Shader::CB_LAMBERT), 0);
	if (FAILED(hr)) {
		goto EXIT;
	}

EXIT:

	return hr;
}

HRESULT ShaderCompiler::CompileTextureDiffuseShader(Shader* p_Shader)
{
	HRESULT hr = E_FAIL;
	extern D3D11User* g_pD3D11User;
	// ******** ���_�V�F�[�_�[���쐬 *********** //

	//���_�v�f�̋L�q�i���_�\���̑��Z�}���e�B�N�X�j
	//�@�@�@���_�\���̂ɐ錾����Ă��鍀�ڂ̎�ށA�T�C�Y���L�q����D3D11_INPUT_ELEMENT_DESC�\����
	D3D11_INPUT_ELEMENT_DESC layout[] = {

		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//����DX11�N���X�Œ��_�V�F�[�_�[�쐬
	hr = g_pD3D11User->CreateVertexShaderFromFile(
		&(p_Shader->m_pVertexShader),  //���_�V�F�[�_�[�I�u�W�F�N�g�ւ̃|�C���^
		_T("TexturedDiffuse.hlsl"),       //�V�F�[�_�[�R�[�h���i�[����Ă���t�@�C�����i�v���W�F�N�g���[�g����̃p�X�Ŏw��j
		"VS_Main",         //�V�F�[�_�[�̎��s���J�n�����G���g���|�C���g�֐���
		"vs_4_0",          //�V�F�[�_�[���f�����w��
		&(p_Shader->m_pLayout),        //���_���C�A�E�g�ւ̃|�C���^
		layout,            //���_���C�A�E�g����邽�߂̒��_���(D3D11_INPUT_ELEMENT_DESC)
		_countof(layout)   //���_���z��
	);
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : Create vertex shader from file failed\n"));
		goto EXIT;
	}
	// ******** �s�N�Z���V�F�[�_�[���쐬 *********** //

	hr = g_pD3D11User->CreatePixelShaderFromFile(
		&(p_Shader->m_pPixelShader),   //�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�ւ̃|�C���^
		_T("TexturedDiffuse.hlsl"),       //�V�F�[�_�[�R�[�h���i�[����Ă���t�@�C�����i�v���W�F�N�g���[�g����̃p�X�Ŏw��j
		"PS_Main",         //�V�F�[�_�[�̎��s���J�n�����G���g���|�C���g�֐���
		"ps_4_0"           //�V�F�[�_�[���f�����w��
	);
	if (FAILED(hr)) {
		OutputDebugString(_T("ERROR : Create pixel shader from file failed\n"));
		goto EXIT;
	}

	//**** �萔�o�b�t�@���쐬 **************************

	//�����o�[�g���˗p�萔�o�b�t�@�\���̂̒萔�o�b�t�@���쐬
	//CPUAccessFlag�̐ݒ��
	hr = g_pD3D11User->CreateConstantBuffer(&(p_Shader->m_pConstantBuffersLambert), NULL, sizeof(Shader::CB_LAMBERT), 0);
	if (FAILED(hr)) {
		goto EXIT;
	}

	// ****** �T���v���[�X�e�[�g�̐ݒ� *****************
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;         //�T���v�����O���Ɏg�p����t�B���^�[���w��B����͈ٕ����t�B���^�[
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;     //0�|�P�͈͊O�ɂ���u�e�N�X�`���[���W�`��@�B����̓��b�s���O�ݒ�B�^�C���Ȃǂ�����
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;     //0�|�P�͈͊O�ɂ���v�e�N�X�`���[���W�`��@�B����̓��b�s���O�ݒ�
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;     //0�|�P�͈͊O�ɂ���w�e�N�X�`���[���W�`��@�B����̓��b�s���O�ݒ�
	samplerDesc.MipLODBias = 0;                            //�v�Z���ꂽ�~�b�v�}�b�v���x������̃o�C�A�X
	samplerDesc.MaxAnisotropy = 16;                        //�T���v�����O�Ɉٕ����t�B���^�����O���g�p���Ă���ꍇ�̌��E�l�B1-16�Ŏw��
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  //��r�I�v�V����
														   //::CopyMemory(samplerDesc.BorderColor, & Utility::Co D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f), sizeof(D3DXVECTOR4)); //���E�F
	samplerDesc.MinLOD = 0;                                //�A�N�Z�X�\�ȃ~�b�v�}�b�v�����l
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                //�A�N�Z�X�\�ȃ~�b�v�}�b�v����l
														   //�T���v���[�X�e�[�g�̍쐬
	hr = g_pD3D11User->m_D3DDevice->CreateSamplerState(&samplerDesc, &(p_Shader->m_pSamplerState));
	if (FAILED(hr)) {
		OutputDebugString(_T("error : Sampler state create failed .... \n"));
		goto EXIT;
	}
	hr = S_OK;

EXIT:

	return hr;
}
//
//HRESULT ShaderCompiler::CompileShaders() 
//{
//	HRESULT hr = E_FAIL;
//	extern D3D11User* g_pD3D11User;
//	// ******** ���_�V�F�[�_�[���쐬 *********** //
//
//	//���_�v�f�̋L�q�i���_�\���̑��Z�}���e�B�N�X�j
//	//�@�@�@���_�\���̂ɐ錾����Ă��鍀�ڂ̎�ށA�T�C�Y���L�q����D3D11_INPUT_ELEMENT_DESC�\����
//	D3D11_INPUT_ELEMENT_DESC layout[] = {
//
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
//	};
//
//	//����DX11�N���X�Œ��_�V�F�[�_�[�쐬
//	hr = g_pD3D11User->CreateVertexShaderFromFile(
//		&m_pVertexShader,  //���_�V�F�[�_�[�I�u�W�F�N�g�ւ̃|�C���^
//		_T("BasicHLSLFbx.hlsl"),       //�V�F�[�_�[�R�[�h���i�[����Ă���t�@�C�����i�v���W�F�N�g���[�g����̃p�X�Ŏw��j
//		"VS_Main",         //�V�F�[�_�[�̎��s���J�n�����G���g���|�C���g�֐���
//		"vs_4_0",          //�V�F�[�_�[���f�����w��
//		&m_pLayout,        //���_���C�A�E�g�ւ̃|�C���^
//		layout,            //���_���C�A�E�g����邽�߂̒��_���(D3D11_INPUT_ELEMENT_DESC)
//		_countof(layout)   //���_���z��
//	);
//	if (FAILED(hr)) {
//		OutputDebugString(_T("ERROR : Create vertex shader from file failed\n"));
//		goto EXIT;
//	}
//	// ******** �s�N�Z���V�F�[�_�[���쐬 *********** //
//
//	hr = g_pD3D11User->CreatePixelShaderFromFile(
//		&m_pPixelShader,   //�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�ւ̃|�C���^
//		_T("BasicHLSLFbx.hlsl"),       //�V�F�[�_�[�R�[�h���i�[����Ă���t�@�C�����i�v���W�F�N�g���[�g����̃p�X�Ŏw��j
//		"PS_Main",         //�V�F�[�_�[�̎��s���J�n�����G���g���|�C���g�֐���
//		"ps_4_0"           //�V�F�[�_�[���f�����w��
//	);
//	if (FAILED(hr)) {
//		OutputDebugString(_T("ERROR : Create pixel shader from file failed\n"));
//		goto EXIT;
//	}
//
//	//**** �萔�o�b�t�@���쐬 **************************
//
//	//�����o�[�g���˗p�萔�o�b�t�@�\���̂̒萔�o�b�t�@���쐬
//	//CPUAccessFlag�̐ݒ��
//	hr = g_pD3D11User->CreateConstantBuffer(&m_pConstantBuffersLambert, NULL, sizeof(CB_LAMBERT), 0);
//	if (FAILED(hr)) {
//		goto EXIT;
//	}
//
//	// ****** �T���v���[�X�e�[�g�̐ݒ� *****************
//	D3D11_SAMPLER_DESC samplerDesc;
//	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;         //�T���v�����O���Ɏg�p����t�B���^�[���w��B����͈ٕ����t�B���^�[
//	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;     //0�|�P�͈͊O�ɂ���u�e�N�X�`���[���W�`��@�B����̓��b�s���O�ݒ�B�^�C���Ȃǂ�����
//	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;     //0�|�P�͈͊O�ɂ���v�e�N�X�`���[���W�`��@�B����̓��b�s���O�ݒ�
//	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;     //0�|�P�͈͊O�ɂ���w�e�N�X�`���[���W�`��@�B����̓��b�s���O�ݒ�
//	samplerDesc.MipLODBias = 0;                            //�v�Z���ꂽ�~�b�v�}�b�v���x������̃o�C�A�X
//	samplerDesc.MaxAnisotropy = 16;                        //�T���v�����O�Ɉٕ����t�B���^�����O���g�p���Ă���ꍇ�̌��E�l�B1-16�Ŏw��
//	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  //��r�I�v�V����
//														   //::CopyMemory(samplerDesc.BorderColor, & Utility::Co D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f), sizeof(D3DXVECTOR4)); //���E�F
//	samplerDesc.MinLOD = 0;                                //�A�N�Z�X�\�ȃ~�b�v�}�b�v�����l
//	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;                //�A�N�Z�X�\�ȃ~�b�v�}�b�v����l
//														   //�T���v���[�X�e�[�g�̍쐬
//	hr = g_pD3D11User->m_D3DDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState);
//	if (FAILED(hr)) {
//		OutputDebugString(_T("error : Sampler state create failed .... \n"));
//		goto EXIT;
//	}
//	hr = S_OK;
//
//EXIT:
//	
//	return hr;
//}
//

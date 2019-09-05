#include "stdafx.h"
#include "Material.h"
#include "ShaderCompiler.h"

Material::Material()
{
}


Material::Material(TCHAR* pShaderName)
{
	extern ShaderCompiler* g_pShaderCompiler;

	//�R���p�C���ς݂̃V�F�[�_�[���\�[�X�ւ̃|�C���^�ɒu��������
	//���s���ɂ����̃V�F�[�_�[�̃��\�[�X���ύX����邱�Ƃ͂Ȃ�����A�O���[�o���ȃ��\�[�X�ւ̃|�C���^��u���ւ��邾���ł���
	m_pConstantBuffersLambert = g_pShaderCompiler->m_shaderMap[*pShaderName]->m_pConstantBuffersLambert;
	m_pLayout = g_pShaderCompiler->m_shaderMap[*pShaderName]->m_pLayout;
	m_pVertexShader = g_pShaderCompiler->m_shaderMap[*pShaderName]->m_pVertexShader;
	m_pPixelShader = g_pShaderCompiler->m_shaderMap[*pShaderName]->m_pPixelShader;
	m_pSamplerState = g_pShaderCompiler->m_shaderMap[*pShaderName]->m_pSamplerState;
}

Material::~Material()
{
}

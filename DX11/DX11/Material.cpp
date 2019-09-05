#include "stdafx.h"
#include "Material.h"
#include "ShaderCompiler.h"

Material::Material()
{
}


Material::Material(TCHAR* pShaderName)
{
	extern ShaderCompiler* g_pShaderCompiler;

	//コンパイル済みのシェーダーリソースへのポインタに置き換える
	//実行中にこれらのシェーダーのリソースが変更されることはないから、グローバルなリソースへのポインタを置き替えるだけでいい
	m_pConstantBuffersLambert = g_pShaderCompiler->m_shaderMap[*pShaderName]->m_pConstantBuffersLambert;
	m_pLayout = g_pShaderCompiler->m_shaderMap[*pShaderName]->m_pLayout;
	m_pVertexShader = g_pShaderCompiler->m_shaderMap[*pShaderName]->m_pVertexShader;
	m_pPixelShader = g_pShaderCompiler->m_shaderMap[*pShaderName]->m_pPixelShader;
	m_pSamplerState = g_pShaderCompiler->m_shaderMap[*pShaderName]->m_pSamplerState;
}

Material::~Material()
{
}

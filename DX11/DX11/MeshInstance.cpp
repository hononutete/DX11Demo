#include "stdafx.h"
#include "MeshInstance.h"
#include "Scene.h"
#include "Camera.h"
#include "ShaderCompiler.h"


MeshInstance::MeshInstance()
{
}


MeshInstance::~MeshInstance()
{
}

//���̃��b�V���C���X�^���X�̕`������s����
HRESULT MeshInstance::Render() {
	HRESULT hr = E_FAIL;
	extern D3D11User* g_pD3D11User;
	extern Scene* g_pScene;
	UINT DrawMeshIndex = 0;

	////���b�V�����Ȃ������ꍇ�����_�����O�͎��s����Ȃ�
	//if (!IsRenderable()) {
	//	return S_OK;
	//}

	//*** ���f���̃A�j���[�V�������X�V ***
	if (m_pMeshGroup->Meshes[0].HasAnimation())
		m_pMeshGroup->Meshes[0].UpdateAnimationTime();


	//********* ���_�֘A�̃f�[�^���f�o�C�X�R���e�L�X�g�ɃZ�b�g�@**************


	//���_�o�b�t�@��ݒ�  --- ���_�o�b�t�@�͂��ꂼ��̒��_�f�[�^�i���W�Ȃǁj�̔z��B�`�悷�邽�߂ɂ͒��_�o�b�t�@���f�o�C�X�ɋ�����
	UINT stride = sizeof(Mesh::GVERTEX);
	UINT offset = 0;
	g_pD3D11User->m_D3DDeviceContext->IASetVertexBuffers(0, 1, &m_pMeshGroup->Meshes[0].VertexBuffer, &stride, &offset);

	//���_�C���f�b�N�X�o�b�t�@��ݒ� --- ���_�C���f�b�N�X�o�b�t�@�̓|���S������邽�߂̒��_�̌��я��ɒ��_�ԍ����i�[���ꂽ�o�b�t�@	
	g_pD3D11User->m_D3DDeviceContext->IASetIndexBuffer(m_pMeshGroup->Meshes[0].IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//���_���C�A�E�g�ݒ�
	g_pD3D11User->m_D3DDeviceContext->IASetInputLayout(m_pMaterial->m_pLayout);

	//�v���~�e�B�u�^�C�v����уf�[�^�̏����Ɋւ������ݒ� ---- �`�悷��v���~�e�B�u�̌`����w��i�W�I���g���V�F�[�_�[�̏ꍇ�V�F�[�_�[���ōĒ�`�����̂ňӖ��Ȃ��炵���j
	//�@*** �t���b�g�V�F�[�f�B���O�̏ꍇ�́A�O�p���X�g���w��
	g_pD3D11User->m_D3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�@*** �O�[���[�V�F�[�f�B���O���g���ꍇ�́A�O�p�X�g���b�v���w��
	//g_pD3D11User->m_D3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	//********** �s��ݒ� ****************************


	//�t���[���ړ�

	//���[���h�A�r���[�A�v���W�F�N�V�����ϊ��s��, WVP�ŏI�ϊ��s��
	XMMATRIX matWorld, matView, matProj, matWVP, matNormal;
	matWorld = XMMatrixIdentity();
	matView = XMMatrixIdentity();
	matProj = XMMatrixIdentity();
	matWVP = XMMatrixIdentity();
	matNormal = XMMatrixIdentity();

	//�ˉe�ϊ��s�� , fovY, aspect, Zn, Zf
	{
		matProj = g_pScene->m_pCameraComponent->LoadProjectionMatrix();
	}

	//�r���[�ϊ��s��
	{
		matView = g_pScene->m_pCameraComponent->LoadViewMatrix();
	}

	//���[���h�ϊ��s��
	{
		XMMATRIX matIdentity, matScale, matTrans, matRot, matRotX, matRotY, matRotZ;
		matScale = XMMatrixIdentity();
		matTrans = XMMatrixIdentity();
		matRot = XMMatrixIdentity();
		matRotX = XMMatrixIdentity();
		matRotY = XMMatrixIdentity();
		matRotZ = XMMatrixIdentity();

		//�P�ʍs��
		matIdentity = XMMatrixIdentity();

		//�X�P�[���ϊ�
		matScale = XMMatrixScaling(m_pTransform->scaleX, m_pTransform->scaleY, m_pTransform->scaleZ);

		//���s�ړ�
		matTrans = XMMatrixTranslation(m_pTransform->transX, m_pTransform->transY, m_pTransform->transZ);

		//��]
		matRotX = XMMatrixRotationX(XMConvertToRadians(m_pTransform->eulerAngles.x));
		matRotY = XMMatrixRotationY(XMConvertToRadians(m_pTransform->eulerAngles.y));
		matRotZ = XMMatrixRotationZ(XMConvertToRadians(m_pTransform->eulerAngles.z));
		//matRotX = XMMatrixRotationX(XMConvertToRadians(m_pTransform->rotX));
		//matRotY = XMMatrixRotationY(XMConvertToRadians(m_pTransform->rotY));
		//matRotZ = XMMatrixRotationZ(XMConvertToRadians(m_pTransform->rotZ));
		matRot = matRotX * matRotY * matRotZ;

		matWorld = matIdentity * matScale * matRot * matTrans;

		//�v�Z���s���ꍇ�A���s�ړ��Ɋւ��Ă͖����A��]�s��Ɋւ��Ă͂��̂܂܎g�p�ł���A�X�P�[���s��Ɋւ��Ă�x��3�{�Ȃ�1/3�{�̂悤�ɋt�����g��
		//�������l������ƈȉ��̂悤�ɋ��߂���
		matNormal = matIdentity * matRot;
	}
	//���[���h�E�r���[�E�ˉe�ϊ��s����쐬
	matWVP = matWorld * matView * matProj;

	//D3DXMATRIX�͍s�D��̍s��A�s�D��Ƃ����͍̂s�P�ʂłЂƂ̒萔���W�X�^�Ɋi�[����Ƃ�������
	//�������V�F�[�_�[���ł͗�D��̃��C�A�E�g�Ŕz�u���Ă��邩��]�u���ė�ƍs�����ւ���K�v������
	//�V�F�[�_�[�ł������s������ׂē���ւ���
	matNormal = XMMatrixTranspose(matNormal);//�@���ϊ��s��
	matWVP = XMMatrixTranspose(matWVP);//WVP�s��

									   //**********�@�萔�o�b�t�@ ****************************

									   //�s���萔�o�b�t�@�ɐݒ�A���@���Q����
									   //�@���ڃR�s�[������@(CopyMemory�֐��Œ��ڎg���AD3DDeviceContext->Map��UnMap���g�p���ă������A�N�Z�X�𐧌��j

									   //�AUpdateSubresource���g�����@
									   //��ɂ�����g�����Ƃ���Ƃ��̃G���[���ł遨Can only invoke UpdateSubresource when the destination Resource was created with D3D11_USAGE_DEFAULT and is not a multisampled Resource.
									   //�萔�o�b�t�@�\���̂ɒl��ݒ�
	extern Scene* g_pScene;

	Shader::CB_LAMBERT cb;
	cb.matWVP = matWVP;
	cb.matNormal = matNormal;
	//TODO: ���C�g�x�N�g���͐��K�����Ă���ݒ肷�邪����́A���ׂẴQ�[���I�u�W�F�N�g�ł��邱�Ƃł͂Ȃ�
	XMVECTOR mLight = XMVectorZero();
	mLight = XMLoadFloat4(&(g_pScene->m_vecLight));
	mLight = XMVector4Normalize(mLight);
	XMFLOAT4 light;
	XMStoreFloat4(&light, mLight);
	cb.vecLight = light;
	cb.ambient = g_pScene->m_Ambient;

	//�萔�o�b�t�@�\���̂̏���VRAM��ɓ]��
	g_pD3D11User->m_D3DDeviceContext->UpdateSubresource(m_pMaterial->m_pConstantBuffersLambert, 0, NULL, &cb, 0, 0);


	// ************ �V�F�[�_�[�֘A ************************

	//���_�V�F�[�_�[
	{
		//���_�V�F�[�_�[���f�o�C�X�ɐݒ� (���_�V�F�[�_�[�ւ̃|�C���^, �V�F�[�_�[�ŃC���^�[�t�F�C�X���g�p����ꍇ�K�v, �V�F�[�_�[�̃C���^�[�t�F�C�X�̃N���X�C���X�^���X��)
		g_pD3D11User->m_D3DDeviceContext->VSSetShader(m_pMaterial->m_pVertexShader, NULL, 0);

		//���_�V�F�[�_�[�ɒ萔�o�b�t�@��ݒ� (�f�o�C�X�̔z��̒��Œ萔�o�b�t�@�̐ݒ���J�n����ʒu, �ݒ肷��o�b�t�@�̐�, �f�o�C�X�ɐݒ肷��萔�o�b�t�@)
		{

			//�����o�[�g���˂̒萔�o�b�t�@�\���̂̒萔�o�b�t�@�𒸓_�V�F�[�_�[�ɃZ�b�g
			g_pD3D11User->m_D3DDeviceContext->VSSetConstantBuffers(0, 1, &m_pMaterial->m_pConstantBuffersLambert);

		}
	}

	//�s�N�Z���V�F�[�_�[
	{
		//�s�N�Z���V�F�[�_�[���f�o�C�X�ɐݒ�
		g_pD3D11User->m_D3DDeviceContext->PSSetShader(m_pMaterial->m_pPixelShader, NULL, 0);

		//�s�N�Z���V�F�[�_�[�ɒ萔�o�b�t�@��ݒ� (�s�N�Z�� �V�F�[�_�[�̃p�C�v���C�� �X�e�[�W�Ŏg�p�����萔�o�b�t�@�[��ݒ�)

		//�t�H���V�F�[�f�B���O�ł͎g�p����
		//g_pD3D11User->m_D3DDeviceContext->PSSetConstantBuffers(1, 1, &g_pConstantBuffers[1]);

		//�V�F�[�_�[�ɃT���v���[�X�e�[�g��ݒ�
		g_pD3D11User->m_D3DDeviceContext->PSSetSamplers(0, 1, &m_pMaterial->m_pSamplerState);

		//�V�F�[�_�[�ɃV�F�[�_�[���\�[�X�r���[��ݒ�i�V�F�[�_�[�p�Ƀ��[�h�����e�N�X�`���j

		//����͕`�悳��郁�b�V���Ŏg�p�����e�N�X�`����ݒ肷��
		ID3D11ShaderResourceView* pSRV = NULL;

		//���b�V���Ŏg�p����Ă���e�N�X�`�����擾
		//TODO: ���������̃e�N�X�`�����g���Ă���ꍇ�S��
		m_pMeshGroup->Meshes[DrawMeshIndex].GetTexture(_T("DiffuseColor"), &pSRV);

		//�s�N�Z���V�F�[�_�[�Ƀe�N�X�`�����\�[�X�r���[��ݒ�
		g_pD3D11User->m_D3DDeviceContext->PSSetShaderResources(0, 1, &pSRV);

		//�f�o�b�O�p
		//g_pD3D11User->m_D3DDeviceContext->PSSetShaderResources(0, 1, &g_pSRView);
	}

	//�n���V�F�[�_�[�𖳌��ɂ���
	g_pD3D11User->m_D3DDeviceContext->HSSetShader(NULL, NULL, 0);

	//�h���C���V�F�[�_�[�𖳌��ɂ���
	g_pD3D11User->m_D3DDeviceContext->DSSetShader(NULL, NULL, 0);

	//�W�I���g���V�F�[�_�[�𖳌��ɂ���
	g_pD3D11User->m_D3DDeviceContext->GSSetShader(NULL, NULL, 0);

	//�R���s���[�g�V�F�[�_�[�𖳌��ɂ���
	g_pD3D11User->m_D3DDeviceContext->CSSetShader(NULL, NULL, 0);


	//���ʕ`��ɕύX���鏈��


	//���X�^���C�U�̐ݒ���擾
	ID3D11RasterizerState* pRasterState = NULL;
	g_pD3D11User->m_D3DDeviceContext->RSGetState(&pRasterState);
	//�����Ӂ@RSGetState�̓f�t�H���g�ł͖��ݒ��NULL���ݒ肳��Ă���BRSGetState��NULL���Ԃ��Ă��Ă��܂��̂ŁA����ď������i�K�ŏ����l��ݒ肵�Ă�������

	D3D11_RASTERIZER_DESC RasterDesc;
	//::ZeroMemory(&RasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	pRasterState->GetDesc(&RasterDesc);

	//�擾�������X�^���C�U�̐ݒ�𗼖ʕ`��ɕύX���čĐݒ�
	//RasterDesc.CullMode = D3D11_CULL_NONE;

	g_pD3D11User->m_D3DDevice->CreateRasterizerState(&RasterDesc, &pRasterState);
	g_pD3D11User->m_D3DDeviceContext->RSSetState(pRasterState);

	SAFE_RELEASE(pRasterState);


	// ************ �`��@************************

	//�C���f�b�N�X�o�b�t�@���g�p�����`��
	g_pD3D11User->m_D3DDeviceContext->DrawIndexed(m_pMeshGroup->Meshes[DrawMeshIndex].IndexCount, 0, 0);


	//�I������
EXIT:
	//SAFE_RELEASE(constantBuffers);

	return hr;


}
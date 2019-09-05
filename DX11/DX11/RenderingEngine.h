#pragma once
#include <vector>
#include "MeshInstance.h"

class RenderingEngine
{
public:
	RenderingEngine();
	~RenderingEngine();

	//���b�V���C���X�^���X�̘A�����X�g
	std::vector<MeshInstance*> m_pMeshInstanceList;

	//���b�V���C���X�^���X���쐬
	void CreateMeshInstance();

	//�Q�[�����[���h��ΏۂɁA���ۂ̃����_�����O���s��
	void RenderFrameAndSwapBuffer();

	//�o�b�`�������s��

	//�J�����O�V�X�e��

	//������J�����O
	void FrustumCulling();

	//�I�N���[�W�����J�����O�i�Օ��J�����O�j
	void OcclusionCulling();

	//�|�[�^���J�����O
	void PortalCulling();
};


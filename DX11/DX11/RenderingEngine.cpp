#include "stdafx.h"
#include "RenderingEngine.h"


RenderingEngine::RenderingEngine()
{
}


RenderingEngine::~RenderingEngine()
{
}

void RenderingEngine::CreateMeshInstance() {

}

void RenderingEngine::RenderFrameAndSwapBuffer() {
	//�����ł̓����_�����O���s���A�V�[���̃����_�����O�̓X�e�[�W��





	//�Q�[���I�u�W�F�N�g�̃����_�����O�̏��������肷��

	//�}�e���A���Ƀ����_�[�L���[���ӂ�

	//�����I�u�W�F�N�g�Ɋւ��ẮA�Ō�Ƀ����_�����O����K�v������

	//��{�I�ȃ����_�����O����
	//0: �w�i�Ȃǂ̃X�J�C�{�b�N�X
	//1: �Q�[���I�u�W�F�N�g�i��艜�ɂ���I�u�W�F�N�g���烌���_�����O���Ă����j
	//2:�@���߃I�u�W�F�N�g

	//�����ōl�����邱�Ƃ́A�J�����O���ǂꂾ����邩�ǂ���
	//�����_�����O����K�v�̂Ȃ��Q�[���I�u�W�F�N�g�������_�����O�Ώۂ���O����@�̎��
	//�E�I�N���[�W�����J�����O�F�Օ����ɉB��Ă���Q�[���I�u�W�F�N�g�������_�����O���Ȃ�
	//�E�w�ʃJ�����O�F�������̃|���S����`�悵�Ȃ��Ƃ�����ʓI�ȃJ�����O�ADirectX�̐ݒ��ON/OFF�ł���
	//�E������J�����O�F�J�����̎�����Ɋ܂܂�Ă��Ȃ��Q�[���I�u�W�F�N�g�������_�����O�Ώۂ��珜�O����
	//�@������J�����O�ł�
}

//������J�����O
void RenderingEngine::FrustumCulling() {
	/*
	������J�����O�ł́A������i�J�����͈̔́j�̊O���ɂ���Q�[���I�u�W�F�N�g�������_�����O�Ώۂ��珜�O����
	���ꂼ��̃Q�[���I�u�W�F�N�g�̓o�E���e�B���O�{�����[���i��{�I�ɂ͋���̃{�����[���A��Ԍv�Z���ȒP������A��������AABB�Ȃǁj
	�Ǝ����䕽�ʂ̊ԂŊȒP�ȃe�X�g���s��
	*/	
}

void RenderingEngine::OcclusionCulling() {
	/*
	�I�N���[�W�����J�����O�Ƃ́A�Օ����ɂ���Ċ��S�Ƀ����_�����O����Ȃ��I�u�W�F�N�g�������_�����O�Ώۂ��珜�O���邱�Ƃ�
	�I�N���[�W�����J�����O�ł́A�����̒��ȂǎՕ����������󋵂ł͔��ɑ厖�����A�Օ������Ȃ��󋵂ł͋t�ɃI�N���[�W�����J�����O�̕��ׂ������Ȃ��Ă��܂�

	���Z�b�g�iPotentially Visible Set, PVS�j�����炩���߂��߂��@���L����
	PVS�Ƃ̓J�����̂ǂ�Ȏ��_�ɑ΂��Ă�������\���̂���Q�[���I�u�W�F�N�g�����X�g�ɂ���

	PVS�̍쐬�̂ЂƂ̕��@�́A���炩���߃��x����̈�ɕ������Ă�������
	�E���[�U�[���蓮�Őݒ肵����A�����Őݒ肷��c�[��������
	*/
}

void RenderingEngine::PortalCulling() {

}

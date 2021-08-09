#pragma once
#include "Mesh.h"
//#include "Transform.h"
#include "Material.h"
//#include "Component.h"
#include <vector>

//�O���錾
//Component�N���X���g����悤�ɂ��邽��
class Component;
class Renderer;
class Transform;

class GameObject
{
public:
	//�Q�[���I�u�W�F�N�g�̌ŗLID
	int uid = 0;

	//����Ԃɂ��邩�ǂ����̃t���O
	bool isActive = true;

	//�����Ă���R���|�[�l���g�����ւ̃|�C���^���X�g
	std::vector<Component*> m_componentsList;

	//�g�����X�t�H�[�����
	Transform* m_pTransform;

	//���b�V�����@
	//�i���̓e�N�X�`���̓��b�V���������Ă���A�{���̓}�e���A������������ׂ��ł���j
	//MeshGroup* m_pMeshGroup;

	//�}�e���A�����i�V�F�[�_�[�j
	//Material* m_pMaterial;

	//�����_���[
	Renderer* m_pRenderer;

	//������
	HRESULT Initialize();

	//���b�V�����Z�b�g����
	//HRESULT SetMesh(MeshGroup* pMeshGroup);

	//�Q�[���I�u�W�F�N�g�������_�����O����
	HRESULT Render();

	//�t���[�����Ƃ̏���������
	void Update();

	//�}�e���A�����A�g�p����V�F�[�_�[�̖��O�ō쐬����
	//HRESULT CreateMaterial(TCHAR shaderName);

	//�Q�[���I�u�W�F�N�g�ɃR���|�[�l���g�������Z�b�g
	HRESULT SetComponent(Component* pComponent);

	//�Q�[���I�u�W�F�N�g����R���|�[�l���g�������폜
	HRESULT RemoveComponent(Component* pComponent);

	//�R���X�g���N�^
	GameObject();

	//�f�X�g���N�^
	~GameObject();

private:
	void Invalidiate();

	bool IsRenderable();

	void CreateTransformComponent();

};


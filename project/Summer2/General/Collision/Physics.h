#pragma once
#include <list>
#include <vector>
#include <memory>
#include "Collidable.h"
/// <summary>
/// �����蔻��̃V���O���g���N���X
/// </summary>
class Collidable;
class CollisionChecker;
class CollisionProcess;
class Physics final
{
public:
	//�C���X�^���X���擾
	static Physics& GetInstance()
	{
		static Physics instance;
		return instance;
	}
	//����������
	void Init();
	//�o�^
	void Entry(std::shared_ptr<Collidable> collidable);
	//����
	void Exit(std::shared_ptr<Collidable> collidable);
	//�Փˏ���
	void Update();


	//�X�V���~�߂�
	void StopUpdate() { m_isUpdate = false; }

	//�X�V���n�߂�
	void StartUpdate() { m_isUpdate = true; }
private:
	//�����蔻��̃`�F�b�N������N���X
	std::shared_ptr<CollisionChecker> m_collChecker;
	//�Փˏ���
	std::shared_ptr<CollisionProcess> m_collProcessor;
private:
	//�V���O���g���̏���
	Physics() = default;
	~Physics();
	Physics(const Physics&) = delete;
	Physics& operator = (const Physics&) = delete;
	Physics(Physics&&) = delete;
	Physics& operator = (Physics&&) = delete;
private:
	//�o�^����Ă���R���C�_�u���̃��X�g
	std::list<std::shared_ptr<Collidable>> m_collidables;

	// OnCollide�̏Փ˒ʒm�̂��߂̃f�[�^
	struct OnCollideInfo
	{
		std::shared_ptr<Collidable> owner;
		std::shared_ptr<Collidable> colider;
		void OnCollide() { owner->OnCollide(colider); }
	};

	//�d��
	void Gravity();

	//�X�V�����邩
	bool m_isUpdate;
};


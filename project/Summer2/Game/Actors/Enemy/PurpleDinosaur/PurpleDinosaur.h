#pragma once
#include "../EnemyBase.h"
class ActorManager;
class Input;
class Camera;
class EnemyManager;
class AttackManager;
class ActorManager;
class PurpleDinosaurStateBase;
class PurpleDinosaur :
    public EnemyBase
{
public:
	PurpleDinosaur(int modelHandle, Vector3 pos);
	~PurpleDinosaur();
	//����������
	void Init()override;
	//�X�V����
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//�Փ˃C�x���g
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//�`��
	void Draw()const override;
	//�X�V�����̊m��
	void Complete() override;
	//�U���̃N�[���^�C�����擾
	int GetAttackCoolTime() const { return m_attackCoolTime; }
	//�U���̃N�[���^�C�����Z�b�g
	void SetAttackCoolTime(int coolTime) { m_attackCoolTime = coolTime; }
	//���W�b�h�{�f�B
	std::shared_ptr<Rigidbody> GetRb() const { return m_rb; }
	//�R���W����
	std::shared_ptr<ColliderBase> GetColl() const { return m_collisionData; }
	//�R���W�����̏�Ԃ�ݒ�
	void SetCollState(CollisionState collState) { m_collState = collState; }
private:
	//�����̏��
	std::shared_ptr<PurpleDinosaurStateBase> m_state;
	//�U���ł���܂ł̃N�[���^�C��
	int m_attackCoolTime;
	//�U���̃N�[���^�C�����X�V
	void UpdateAttackCoolTime();
};


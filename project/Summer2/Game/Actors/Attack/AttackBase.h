#pragma once
#include <list>
#include "../Actor.h"
#include "../../../General/Battle.h"
class ActorStateBase;
class AttackBase abstract:
    public Actor 
{
public:
	AttackBase(Shape shape,std::weak_ptr<ActorStateBase> ownerState);
	virtual ~AttackBase();

	//�Փ˃C�x���g
	virtual void OnCollide(const std::shared_ptr<Collidable> other)override;
	//�X�V�����̊m��
	void Complete() override {};
	//�m�b�N�o�b�N
	virtual Vector3 GetKnockBackVec(Position3 other);


	//�܂Ƃ߂Đݒ�
	void AttackSetting(int damage,int keepFrame,int knockBackPower, Battle::AttackWeight aw);
	//�_���[�W
	int GetDamage() { return m_damage; };
	void SetDamage(int damage) { m_damage = damage; };
	//�����t���[��
	void SetKeepFrame(int keepFrame) { m_keepFrame = keepFrame; };
	//�m�b�N�o�b�N�̑傫��
	float GetKnockBackPower() { return m_knockBackPower; };
	void SetKnockBackPower(float knockBackPower) { m_knockBackPower = knockBackPower; };
	//�U���̋���
	Battle::AttackWeight GetAttackWeight() { return m_attackWeight; };
	void SetAttackWeight(Battle::AttackWeight ap) { m_attackWeight = ap; };
	//���Ă��A�N�^�[��ID�����Z�b�g
	void ResetHitId() { m_hitId.clear(); };
protected:
	//�_���[�W
	int m_damage;
	//�����t���[��
	int m_keepFrame;
	//�m�b�N�o�b�N�̑傫��
	float m_knockBackPower;
	//�U���̋���
	Battle::AttackWeight m_attackWeight;
	//������̃X�e�[�g
	std::weak_ptr<ActorStateBase> m_ownerState;
	//���Ă����Ƃ̂���Collidable���o���Ă���
	std::list<int> m_hitId;
};


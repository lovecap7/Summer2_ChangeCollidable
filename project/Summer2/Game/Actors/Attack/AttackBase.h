#pragma once
#include "../Actor.h"
#include "../../General/Battle.h"
class ActorStateBase;
class AttackBase abstract:
    public Actor 
{
public:
	AttackBase(Shape shape,int damage,int keepFrame,float knockBackPower, Battle::AttackWeight aw, std::weak_ptr<ActorStateBase> ownerState);
	virtual ~AttackBase();
	//�m�b�N�o�b�N
	virtual Vector3 GetKnockBackVec(Vector3 other) abstract;
	//�_���[�W
	int GetDamage() { return m_damage; };
	void SetDamage(int damage) { m_damage = damage; };
	//�����t���[��
	void SetKeepFrame(int keepFrame) { m_keepFrame = keepFrame; };
	//�m�b�N�o�b�N�̑傫��
	float GetKnockBackPower() { return m_knockBackPower; };
	void SetKnockBackPower(float knockBackPower) { m_knockBackPower = knockBackPower; };
	//�U���̋���
	Battle::AttackWeight GetAttackPower() { return m_attackWeight; };
	void SetAttackPower(Battle::AttackWeight ap) { m_attackWeight = ap; };
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
};


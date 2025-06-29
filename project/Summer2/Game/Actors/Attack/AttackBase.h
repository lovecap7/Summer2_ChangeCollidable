#pragma once
#include <list>
#include "../Actor.h"
#include "../../../General/Battle.h"

enum class AttackType
{
	Slash,
	Strike,
	AreaOfEffect,
	Bullet,
	Blast,
};

class CharacterStateBase;
class Actor;
class AttackBase abstract:
    public Actor 
{
public:
	AttackBase(Shape shape, std::weak_ptr<Actor> owner);
	virtual ~AttackBase();
	//����������
	virtual void Init()override;
	//�X�V����
	virtual void Update() ;
	//�Փ˃C�x���g
	virtual void OnCollide(const std::shared_ptr<Collidable> other)override;
	//�X�V�����̊m��
	void Complete() override {};
	//���S����
	void Dead(const std::weak_ptr<ActorManager> actorManager) override {};
	//�m�b�N�o�b�N
	virtual Vector3 GetKnockBackVec(Position3 other);

	//�܂Ƃ߂Đݒ�
	void AttackSetting(int damage,int keepFrame,int knockBackPower, Battle::AttackWeight aw);
	//�_���[�W
	int GetDamage();
	void SetDamage(int damage) { m_damage = damage; };
	//�����t���[��
	void SetKeepFrame(int keepFrame) { m_keepFrame = keepFrame; };
	//�m�b�N�o�b�N�̑傫��
	float GetKnockBackPower() { return m_knockBackPower; };
	void SetKnockBackPower(float knockBackPower) { m_knockBackPower = knockBackPower; };
	//�U���̋���
	Battle::AttackWeight GetAttackWeight();
	void SetAttackWeight(Battle::AttackWeight ap) { m_attackWeight = ap; };
	//���Ă��A�N�^�[��ID�����Z�b�g
	void ResetHitId() { m_hitId.clear(); };
	//�U�����폜����
	void Delete() { m_isDelete = true; };
	//������̃^�O
	GameTag GetOwnerTag() { return m_ownerTag; };
protected:
	//�_���[�W
	int m_damage;
	//�����t���[��
	int m_keepFrame;
	//�m�b�N�o�b�N�̑傫��
	float m_knockBackPower;
	//�U���̋���
	Battle::AttackWeight m_attackWeight;
	//���Ă����Ƃ̂���Collidable���o���Ă���
	std::list<int> m_hitId;
	//������̎Q��
	std::weak_ptr<Actor> m_owner;
	//������̃^�O
	GameTag m_ownerTag;
	
};


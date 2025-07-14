#pragma once
#include <list>
#include "../Actor.h"
#include "../../../General/Battle.h"
#include "../../Camera/Camera.h"

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
	virtual void Update(const std::weak_ptr<ActorManager> actorManager) ;
	//�Փ˃C�x���g
	virtual void OnCollide(const std::shared_ptr<Collidable> other)override;
	//�X�V�����̊m��
	void Complete() override {};
	//���S����
	void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override {};
	//�I������
	virtual void End()override;
	//�m�b�N�o�b�N
	virtual Vector3 GetKnockBackVec(Position3 other);

	//�܂Ƃ߂Đݒ�
	void AttackSetting(int damage,int keepFrame,int knockBackPower, Battle::AttackWeight aw, int hitStopFrame, ShakePower sp);
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
	//�q�b�g�X�g�b�v�̒���
	void SetHitStopFrame(int frame) { m_hitStopFrame = frame; };
	//�J�����̗h��
	void SetShakePower(ShakePower sp) { m_shakePower = sp; };

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
	//�q�b�g�X�g�b�v�̒���
	int m_hitStopFrame;
	//�J�����̗h��
	ShakePower m_shakePower;
	//���Ă����Ƃ̂���Collidable���o���Ă���
	std::list<int> m_hitId;
	//������̎Q��
	std::weak_ptr<Actor> m_owner;
	//������̃^�O
	GameTag m_ownerTag;
	//���������t���O(����Update�ɓ����������Ƃŏ�����ς��邽�߂̃t���O)
	bool m_isHit;
	//�U��������������(�����������̏�����x�点�邽�߂̃t���O)
	bool m_isSuccessAttack;
};


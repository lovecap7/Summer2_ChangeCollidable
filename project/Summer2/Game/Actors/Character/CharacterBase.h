#pragma once
#include "../Actor.h"
#include <memory>
class CharacterStateBase;
class HitPoints;
class AttackPoints;
class CharacterBase abstract:
    public Actor
{
public:
	CharacterBase(Shape shape);
	virtual ~CharacterBase() {};
    //�̗͂̃X�e�[�^�X
    std::weak_ptr<HitPoints> GetHitPoints() const { return m_hitPoints; };
	//�U���̃X�e�[�^�X
    std::weak_ptr<AttackPoints> GetAttackPoints() const { return m_attackPoints; };
    //�U�����󂯂��Ƃ��̏���
    virtual void OnHitFromAttack(const std::shared_ptr<Collidable> other);
	//�^�[�Q�b�g�Ɋւ���f�[�^�\����
	struct TargetData
	{
		//�^�[�Q�b�g�𔭌�
		bool isHitTarget = false;
		//�^�[�Q�b�g�̍��W
		Position3 targetPos = Vector3::Zero();
		//�^�[�Q�b�g�ւ̌���
		Vector3 targetDirXZ = Vector3::Zero();
		//�^�[�Q�b�g�Ƃ̋���
		float targetDis = 0.0f;
	};
	//�^�[�Q�b�g������
	void LookAtTarget();
protected:
    //�L�����N�^�[�̏��
    std::shared_ptr<CharacterStateBase> m_state;
    //�̗�
    std::shared_ptr<HitPoints> m_hitPoints;
	//�U���̃X�e�[�^�X
	std::shared_ptr<AttackPoints> m_attackPoints;
	//�^�[�Q�b�g�̏��
	TargetData m_targetData;
	//�^�[�Q�b�g�̍��G
	virtual void TargetSearch(float searchDistance, float searchAngle, Vector3 targetPos);
};


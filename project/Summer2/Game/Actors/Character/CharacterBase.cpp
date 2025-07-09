#include "CharacterBase.h"
#include "../../../General/HitPoints.h"
#include "../Attack/AttackBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Model.h"

CharacterBase::CharacterBase(Shape shape) :
	Actor(shape),
	m_targetData{},
	m_isActive(true)
{
}

void CharacterBase::OnHitFromAttack(const std::shared_ptr<Collidable> other)
{
	if (m_hitPoints->IsDead())return;//�̗͂�0�Ȃ牽�����Ȃ�
	//�����Ɠ����^�O�Ȃ��΂�
	if (std::dynamic_pointer_cast<AttackBase>(other)->GetOwnerTag() == m_tag)return;
	auto attack = std::dynamic_pointer_cast<AttackBase>(other);
	//�U�����󂯂��̂Ńt���O�𗧂Ă�
	m_hitPoints->SetIsHit(true);
	//�U���̃_���[�W���󂯂�
	m_hitPoints->Damage(attack->GetDamage());
	//���f���̐F�ƃT�C�Y��ύX
	m_model->ModelHit();
	//�_���[�W���󂯂��甽�����邩���`�F�b�N
	if (Battle::CheckFlinchAttackAndArmor(attack->GetAttackWeight(), m_hitPoints->GetArmor()))
	{
		m_hitPoints->SetIsHitReaction(true);				//��������
		m_rb->AddVec(attack->GetKnockBackVec(m_rb->m_pos));	//�m�b�N�o�b�N
	}
}


void CharacterBase::TargetSearch(float searchDistance, float searchAngle, Vector3 targetPos)
{
	//���Z�b�g
	m_targetData.isHitTarget = false;
	//�������m�F
	auto dir = targetPos.XZ() - m_rb->GetPos().XZ();
	if (dir.Magnitude() <= searchDistance)
	{
		//����p���Ƀ^�[�Q�b�g�����邩
		auto angle = abs(Vector2::GetRad(m_model->GetDir().XZ(), dir)) * MyMath::RAD_2_DEG;
		if (angle <= searchAngle)
		{
			m_targetData.isHitTarget = true;
			m_targetData.targetPos = targetPos;
			m_targetData.targetDirXZ = dir.XZ().Normalize();
			m_targetData.targetDis = dir.Magnitude();
		}
	}
}

void CharacterBase::LookAtTarget()
{
	m_model->SetDir(m_targetData.targetDirXZ.XZ());
}

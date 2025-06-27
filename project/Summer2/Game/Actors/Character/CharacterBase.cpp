#include "CharacterBase.h"
#include "../../../General/HitPoints.h"
#include "../Attack/AttackBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Model.h"

CharacterBase::CharacterBase(Shape shape):
	Actor(shape)
{
}

void CharacterBase::OnHitFromAttack(const std::shared_ptr<Collidable> other)
{
	if (m_hitPoints->IsDead())return;//�̗͂�0�Ȃ牽�����Ȃ�
	auto attack = std::dynamic_pointer_cast<AttackBase>(other);
	//�U�����󂯂��̂Ńt���O�𗧂Ă�
	m_hitPoints->SetIsHit(true);
	//�U���̃_���[�W���󂯂�
	m_hitPoints->Damage(attack->GetDamage());
	//�_���[�W���󂯂��甽�����邩���`�F�b�N
	if (Battle::CheckFlinchAttackAndArmor(attack->GetAttackWeight(), m_hitPoints->GetArmor()))
	{
		m_hitPoints->SetIsHitReaction(true);				//��������
		m_rb->AddVec(attack->GetKnockBackVec(m_rb->m_pos));	//�m�b�N�o�b�N
		m_model->ModelHit();								//�ԐF��
	}
}

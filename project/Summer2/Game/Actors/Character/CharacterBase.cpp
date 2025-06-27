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
	if (m_hitPoints->IsDead())return;//体力が0なら何もしない
	auto attack = std::dynamic_pointer_cast<AttackBase>(other);
	//攻撃を受けたのでフラグを立てる
	m_hitPoints->SetIsHit(true);
	//攻撃のダメージを受ける
	m_hitPoints->Damage(attack->GetDamage());
	//ダメージを受けたら反応するかをチェック
	if (Battle::CheckFlinchAttackAndArmor(attack->GetAttackWeight(), m_hitPoints->GetArmor()))
	{
		m_hitPoints->SetIsHitReaction(true);				//反応する
		m_rb->AddVec(attack->GetKnockBackVec(m_rb->m_pos));	//ノックバック
		m_model->ModelHit();								//赤色に
	}
}

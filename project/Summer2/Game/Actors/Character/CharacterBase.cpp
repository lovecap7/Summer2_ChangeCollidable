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
	if (m_hitPoints->IsDead())return;//体力が0なら何もしない
	//自分と同じタグなら飛ばす
	if (std::dynamic_pointer_cast<AttackBase>(other)->GetOwnerTag() == m_tag)return;
	auto attack = std::dynamic_pointer_cast<AttackBase>(other);
	//攻撃を受けたのでフラグを立てる
	m_hitPoints->SetIsHit(true);
	//攻撃のダメージを受ける
	m_hitPoints->Damage(attack->GetDamage());
	//モデルの色とサイズを変更
	m_model->ModelHit();
	//ダメージを受けたら反応するかをチェック
	if (Battle::CheckFlinchAttackAndArmor(attack->GetAttackWeight(), m_hitPoints->GetArmor()))
	{
		m_hitPoints->SetIsHitReaction(true);				//反応する
		m_rb->AddVec(attack->GetKnockBackVec(m_rb->m_pos));	//ノックバック
	}
}


void CharacterBase::TargetSearch(float searchDistance, float searchAngle, Vector3 targetPos)
{
	//リセット
	m_targetData.isHitTarget = false;
	//距離を確認
	auto dir = targetPos.XZ() - m_rb->GetPos().XZ();
	if (dir.Magnitude() <= searchDistance)
	{
		//視野角内にターゲットがいるか
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

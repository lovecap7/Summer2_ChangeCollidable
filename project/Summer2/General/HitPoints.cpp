#include "HitPoints.h"
#include "../Game/Actors/Attack/AttackBase.h"
#include "Math/MyMath.h"

HitPoints::HitPoints(int hp, Battle::Armor armor) :
	m_hp(hp),
	m_maxHp(hp),
	m_armor(armor),
	m_damageCutRate(1.0f),
	m_isHit(false),
	m_isHitReaction(false),
	m_isNoDamage(false)
{
}

HitPoints::~HitPoints()
{
}

void HitPoints::Init()
{
	m_isHit = false;
	m_isHitReaction = false;
}

void HitPoints::OnHitAttack(const std::shared_ptr<AttackBase> attack)
{
	if (IsDead())return;
	//攻撃を受けたのでフラグを立てる
	m_isHit = true;
	//攻撃のダメージを受ける
	Damage(attack->GetDamage());
	//ダメージを受けたら反応するかをチェック
	if (Battle::CheckFlinchAttackAndArmor(attack->GetAttackWeight(), m_armor))
	{
		m_isHitReaction = true;	//反応する
	}
}

void HitPoints::Heal(int heal)
{
	if (m_hp >= m_maxHp)return;	//最大なら
	if (heal < 0.0f)heal *= -1.0f;
	m_hp += heal;
	m_hp = MathSub::ClampInt(m_hp, 0, m_maxHp);
}

void HitPoints::Damage(int damage)
{
	if (m_isNoDamage)return;	//無敵なら
	if (damage < 0.0f)damage *= -1.0f;
	m_hp -= damage * m_damageCutRate;
	if (m_hp <= 0)
	{
		m_hp = 0;
	}
}

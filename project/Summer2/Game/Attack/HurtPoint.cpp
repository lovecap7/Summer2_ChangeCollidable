#include "HurtPoint.h"
#include "../Actors/Actor.h"
#include "../../General/Collidable.h"
#include "../../General/Rigidbody.h"
#include "AttackBase.h"

HurtPoint::HurtPoint(std::shared_ptr<Collidable> coll, int hp, std::shared_ptr<Actor> owner):
	m_collidable(coll),
	m_isNoDamage(false),
	m_hp(hp),
	m_maxHp(hp),
	m_owner(owner),
	m_isHit(false),
	m_isHitReaction(false),
	m_armor(Battle::Armor::Light),
	m_damageCutRate(1.0f)
{
}

HurtPoint::~HurtPoint()
{

}

void HurtPoint::Init()
{
	//初期化
	m_isHit = false;
	m_isHitReaction = false;
}

void HurtPoint::OnHit(std::shared_ptr<AttackBase> attack)
{
	//無敵なら
	if (m_isNoDamage)return;
	//攻撃を喰らった
	m_isHit = true;
	//ダメージを喰らう
	OnHitDamage(attack->GetDamage());
	//リアクションをするか
	if (Battle::CheckFlinchAttackAndArmor(attack->GetAttackPower(), m_armor))
	{
		//ヒットリアクションをする
		m_isHitReaction = true;	
		//ノックバック
		auto vec = attack->GetKnockBackVec(m_owner->GetCollidable()->GetRb()->GetNextPos());
		OnHitKnockBack(vec);
	}
}

void HurtPoint::OnHitDamage(int damage)
{
	m_hp -= damage * m_damageCutRate;
	if (m_hp <= 0)
	{
		m_hp = 0;
	}
}

void HurtPoint::OnHitKnockBack(const Vector3& knockBackVec)
{
	m_owner->GetCollidable()->GetRb()->SetMoveVec(knockBackVec);//のけぞる
}

void HurtPoint::AddHp(int add)
{
	if (m_hp >= m_maxHp)return;//最大なら
	m_hp += add;
	m_hp = MathSub::ClampInt(m_hp, 0, m_maxHp);
}

void HurtPoint::SetDamageCutRate(float damageCutRate)
{
	m_damageCutRate = damageCutRate;
}

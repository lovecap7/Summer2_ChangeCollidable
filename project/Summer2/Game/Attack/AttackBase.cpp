#include "AttackBase.h"
#include "../Actors/Actor.h"
#include "../../General/Collidable.h"
#include "../../General/Rigidbody.h"

AttackBase::AttackBase(std::shared_ptr<Collidable> coll, int damage, int keepFrame, float knockBackPower, std::shared_ptr<Actor> owner):
	m_collidable(coll),
	m_damage(damage),
	m_keepFrame(keepFrame),
	m_knockBackPower(knockBackPower),
	m_isDelete(false),
	m_owner(owner),
	m_initDamage(damage),
	m_initKeepFrame(keepFrame),
	m_attackPower(Battle::AttackWeight::Middle)
{
}

void AttackBase::SetDamage(int damage)
{
	m_initDamage = damage;
	m_damage = damage;
}

void AttackBase::SetKeepFrame(int keepFrame)
{
	m_initKeepFrame = keepFrame;
	m_keepFrame = keepFrame;
}

Vector3 AttackBase::GetKnockBackVec(Vector3 other)
{
	//ノックバック
	Vector3 knockBackVec = other - m_owner->GetCollidable()->GetRb()->GetNextPos();//離れるベクトル
	knockBackVec.y = 0.0f;//Y成分はなし
	knockBackVec = knockBackVec.Normalize() * m_knockBackPower;//ノックバック
	return knockBackVec;
}

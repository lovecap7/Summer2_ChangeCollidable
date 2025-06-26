#include "AttackBase.h"
#include "../ActorStateBase.h"

AttackBase::AttackBase(Shape shape, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, std::weak_ptr<ActorStateBase> ownerState):
	Actor(shape),
	m_damage(damage),
	m_keepFrame(keepFrame),
	m_knockBackPower(knockBackPower),
	m_attackWeight(aw),
	m_ownerState(ownerState)
{
}

AttackBase::~AttackBase()
{
}

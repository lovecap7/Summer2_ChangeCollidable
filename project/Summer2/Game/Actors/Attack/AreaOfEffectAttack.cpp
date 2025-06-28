#include "AreaOfEffectAttack.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Rigidbody.h"

AreaOfEffectAttack::AreaOfEffectAttack(std::weak_ptr<Actor> owner):
	SphereAttackBase(owner)
{
}

AreaOfEffectAttack::~AreaOfEffectAttack()
{
}

void AreaOfEffectAttack::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	AttackBase::Update();
}

void AreaOfEffectAttack::Draw() const
{
#if _DEBUG
	auto coll = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(),
		coll->m_radius, 16, 0xff0000, 0xff0000, false);
#endif
}

#include "Slash.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"

Slash::Slash(std::weak_ptr<Actor> owner):
	CapsuleAttackBase(owner)
{
}

Slash::~Slash()
{
}

void Slash::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	AttackBase::Update();
}

void Slash::Draw() const
{
#if _DEBUG
	auto coll = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	DrawCapsule3D(m_rb->m_pos.ToDxLibVector(), coll->m_endPos.ToDxLibVector(),
		coll->m_radius,16,0xff0000,0xff0000,false);
#endif
}

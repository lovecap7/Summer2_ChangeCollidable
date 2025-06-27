#include "Strike.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
Strike::Strike(std::weak_ptr<Actor> owner):
	CapsuleAttackBase(owner)
{
}

Strike::~Strike()
{
}

void Strike::Draw() const
{
#if _DEBUG
	auto coll = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	DrawCapsule3D(m_rb->m_pos.ToDxLibVector(), coll->m_endPos.ToDxLibVector(),
		coll->m_radius, 16, 0xff0000, 0xff0000, false);
#endif
}

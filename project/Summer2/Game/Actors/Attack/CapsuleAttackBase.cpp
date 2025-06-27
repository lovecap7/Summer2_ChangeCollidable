#include "CapsuleAttackBase.h"
#include "../../../General/Collision/CapsuleCollider.h"

CapsuleAttackBase::CapsuleAttackBase(std::weak_ptr<ActorStateBase> ownerState, int& modelHandle, int startIndex, int endIndex, float radius) :
	AttackBase(Shape::Capsule, ownerState),
	m_modelHandle(modelHandle),
	m_startIndex(startIndex),
	m_endIndex(endIndex)
{
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetRadius(radius);
}

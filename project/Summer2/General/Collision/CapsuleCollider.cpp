#include "CapsuleCollider.h"

CapsuleCollider::CapsuleCollider(Position3 endPos, float radius):
	ColliderBase(Shape::Capsule),
	m_endPos(endPos),
	m_radius(radius),
	m_hitNearPos(0.0f, 0.0f, 0.0f)
{
}
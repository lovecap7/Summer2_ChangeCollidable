#include "SphereCollider.h"

SphereCollider::SphereCollider(float radius):
	ColliderBase(Shape::Sphere),//���̌`
	m_radius(radius)
{
}

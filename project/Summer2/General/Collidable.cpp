#include "Collidable.h"
#include "Collision/SphereCollider.h"
#include "Collision/CapsuleCollider.h"
#include "Collision/PolygonCollider.h"

Collidable::Collidable(State state, Priority priority, GameTag gameTag, Shape shape, bool isTrigger):
	m_state(state),
	m_priority(priority),
	m_tag(gameTag),
	m_isTrigger(isTrigger)
{
	CreateCollider(shape); // コライダーを作成
}

Collidable::~Collidable()
{
}

std::shared_ptr<ColliderBase> Collidable::CreateCollider(Shape shape)
{
	std::shared_ptr<ColliderBase> coll;
	if (shape == Shape::Sphere)
	{
		coll = std::make_shared<SphereCollider>();
	}
	else if (shape == Shape::Capsule)
	{
		coll = std::make_shared<CapsuleCollider>();
	}
	else if (shape == Shape::Polygon)
	{
		coll = std::make_shared<PolygonCollider>();
	}

	return coll;
}

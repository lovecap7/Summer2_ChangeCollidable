#include "Collidable.h"
#include "../Rigidbody.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PolygonCollider.h"
#include "Physics.h"
Collidable::Collidable(Shape shape):
	m_isThrough(false),
	m_isTrigger(false),
	m_collState(CollisionState::Normal),
	m_priority(Priority::Middle),
	m_tag(GameTag::None),
	m_isFloor(false),
	m_isWall(false)
{
	CreateCollider(shape);
	m_rb = std::make_shared<Rigidbody>();
}

void Collidable::Init()
{
	//Physicsに登録
	Physics::GetInstance().Entry(shared_from_this());
}

void Collidable::End()
{
	//Physicsから登録解除
	Physics::GetInstance().Exit(shared_from_this());
}

void Collidable::CreateCollider(Shape shape)
{
	switch (shape)
	{
	case Shape::Sphere:
		m_collisionData = std::make_shared<SphereCollider>();
		break;
	case Shape::Capsule:
		m_collisionData = std::make_shared<CapsuleCollider>();
		break;
	case Shape::Polygon:
		m_collisionData = std::make_shared<PolygonCollider>();
		break;
	default:
		m_collisionData = nullptr;
		break;
	}
}

void Collidable::AllSetting(CollisionState collState, Priority priority, GameTag tag, bool isTrough, bool isTrigger, bool isGravity)
{
	m_collState = collState;		//状態
	m_priority = priority;			//優先度
	m_tag = tag;					//タグ
	m_isThrough = isTrough;			//当たり判定を無視するか
	m_isTrigger = isTrigger;		//トリガー判定をするか
	m_rb->m_isGravity = isGravity;	//重力を受けるか
}

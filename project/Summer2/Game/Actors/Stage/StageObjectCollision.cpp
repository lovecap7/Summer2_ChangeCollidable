#include "StageObjectCollision.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"

StageObjectCollision::StageObjectCollision(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle) :
	Actor(Shape::Polygon),
	m_collisionHandle(modelHandle)
{
	//‰ŠúˆÊ’u
	m_rb->SetPos(pos);
	std::dynamic_pointer_cast<PolygonCollider>(m_collisionData)->m_modelHandle = m_collisionHandle;
	DxLib::MV1SetScale(m_collisionHandle, scale);
	DxLib::MV1SetRotationXYZ(m_collisionHandle, angle);
}


StageObjectCollision::StageObjectCollision(VECTOR pos, float radius):
	Actor(Shape::Sphere),
	m_collisionHandle(-1)
{
	std::dynamic_pointer_cast<SphereCollider>(m_collisionData)->SetRadius(radius);
	m_rb->SetPos(pos);
}

StageObjectCollision::StageObjectCollision(VECTOR pos1, VECTOR pos2, float radius):
	Actor(Shape::Capsule),
	m_collisionHandle(-1)
{
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetEndPos(pos2);
	cap->SetRadius(radius);
	m_rb->SetPos(pos1);
}


StageObjectCollision::~StageObjectCollision()
{
	//‚È‚µ
}

void StageObjectCollision::Init()
{
	m_collState = CollisionState::Normal;
	m_priority = Priority::Static;
	m_tag = GameTag::Object;
	Collidable::Init();
}
void StageObjectCollision::Update(const std::weak_ptr<Camera> camera)
{
	//‚È‚µ
}

void StageObjectCollision::OnCollide(const std::shared_ptr<Collidable> other)
{
	//‚È‚µ
}

void StageObjectCollision::Draw() const
{
	//‚È‚µ
}

void StageObjectCollision::Complete()
{
	//‚È‚µ
}

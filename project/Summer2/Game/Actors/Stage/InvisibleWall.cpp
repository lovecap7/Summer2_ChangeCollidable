#include "InvisibleWall.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"

InvisibleWall::InvisibleWall(int modelHandle, Vector3 pos, VECTOR scale, VECTOR angle):
	Actor(Shape::Polygon),
	m_modelHandle(modelHandle)
{
	//初期位置
	m_rb->SetPos(pos);
	std::dynamic_pointer_cast<PolygonCollider>(m_collisionData)->m_modelHandle = modelHandle;
	DxLib::MV1SetScale(m_modelHandle, scale);
	DxLib::MV1SetRotationXYZ(m_modelHandle, angle);
}

InvisibleWall::~InvisibleWall()
{
}

void InvisibleWall::Init()
{
	//コライダブルの初期化
	m_collState = CollisionState::Normal;
	m_priority = Priority::Static;
	m_tag = GameTag::Object;
	Collidable::Init();
}

void InvisibleWall::Update(const std::weak_ptr<Camera> camera)
{
	DxLib::MV1SetPosition(m_modelHandle, m_rb->GetPos().ToDxLibVector());
}

void InvisibleWall::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void InvisibleWall::Draw() const
{
#if _DEBUG
	//DxLib::MV1DrawModel(m_modelHandle);
#endif
}

void InvisibleWall::Complete()
{
}

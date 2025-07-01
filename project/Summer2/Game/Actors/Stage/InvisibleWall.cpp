#include "InvisibleWall.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"

InvisibleWall::InvisibleWall(int modelHandle, Vector3 pos, VECTOR scale, VECTOR angle):
	Actor(Shape::Polygon),
	m_modelHandle(modelHandle)
{
	//�����ʒu
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
	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::Static, GameTag::Object, false, false,false);
	Collidable::Init();
}

void InvisibleWall::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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

void InvisibleWall::End()
{
	Collidable::End();
	MV1DeleteModel(m_modelHandle);
}

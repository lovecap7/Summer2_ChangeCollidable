#include "StageObjectCollision.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Model.h"

StageObjectCollision::StageObjectCollision(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle) :
	Actor(Shape::Polygon),
	m_collisionHandle(modelHandle)
{
	//���f��
	m_model = std::make_shared<Model>(modelHandle, pos);
	m_model->SetScale(scale);
	m_model->SetRot(angle);
	//�����ʒu
	m_rb->SetPos(pos);
	std::dynamic_pointer_cast<PolygonCollider>(m_collisionData)->m_modelHandle = m_collisionHandle;
	//DxLib::MV1SetScale(m_collisionHandle, scale);
	//DxLib::MV1SetRotationXYZ(m_collisionHandle, angle);
	//DxLib::MV1SetPosition(m_collisionHandle, pos);
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
	//�Ȃ�
}

void StageObjectCollision::Init()
{
	//�R���C�_�u���̏�����
	AllSetting(CollisionState::Normal, Priority::Static, GameTag::Object, false, false,false);
	Collidable::Init();
	MV1SetupCollInfo(m_collisionHandle, -1);
}
void StageObjectCollision::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	MV1RefreshCollInfo(m_collisionHandle, -1);
}

void StageObjectCollision::OnCollide(const std::shared_ptr<Collidable> other)
{
	//�Ȃ�
}

void StageObjectCollision::Draw() const
{
	m_model->Draw();
}

void StageObjectCollision::Complete()
{
	//�Ȃ�
}
